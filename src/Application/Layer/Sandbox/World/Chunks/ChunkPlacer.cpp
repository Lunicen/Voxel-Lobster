#include "ChunkPlacer.h"

#include "Structure/ChunkMeshUtils.h"

std::mutex ChunkPlacer::_chunksMutex;

std::vector<std::tuple<Position, ChunkBlocks, std::vector<Vertex>>> ChunkPlacer::_chunksToLoad = {};
std::atomic<bool> ChunkPlacer::_hasPositionChanged;
std::condition_variable ChunkPlacer::_lazyLoaderLock;

std::vector<std::unique_ptr<Chunk>> ChunkPlacer::_freeChunks = {};
std::unordered_map<Position, std::unique_ptr<Chunk>> ChunkPlacer::_loadedChunks = {};
Position ChunkPlacer::_previousNormalizedPosition = {};
std::atomic<bool> ChunkPlacer::_running;

std::shared_ptr<WorldGenerator> ChunkPlacer::_generator = nullptr;
std::unique_ptr<Order> ChunkPlacer::_order = nullptr;

Position ChunkPlacer::GetNormalizedPosition(const Point3D& position, const size_t& chunkSize) const
{
	auto normalizedPosition = position;

	normalizedPosition /= chunkSize;

	normalizedPosition.x = floor(normalizedPosition.x);
	normalizedPosition.y = floor(normalizedPosition.y);
	normalizedPosition.z = floor(normalizedPosition.z);

	return {normalizedPosition};
}

std::string ChunkPlacer::PositionToString(const Position& position) const
{
	return std::to_string(position.x) + ", " + 
		   std::to_string(position.y) + ", " + 
		   std::to_string(position.z);
}

bool ChunkPlacer::AddNewChunks(const std::unordered_set<Position>& currentChunkOrigins)
{
	const auto size = _order->GetChunkSize();

	for(auto origin : currentChunkOrigins)
	{
		if (_hasPositionChanged || !_running)
		{
			return true;
		}

		if (_loadedChunks.find(origin) == _loadedChunks.end())
		{
			const auto chunkFrame = ChunkFrame{origin, size};

			ChunkBlocks chunkBlocks;
			chunkBlocks.resize(size * size * size);
			
			_generator->PaintChunk(chunkFrame, chunkBlocks);

			auto mesh = ChunkMeshUtils::GetMeshVertices(chunkFrame, chunkBlocks, _generator->GetBlockMap());
			
			const std::lock_guard<std::mutex> lock(_chunksMutex);
			_chunksToLoad.emplace_back(origin, chunkBlocks, mesh);
		}
	}

	return false;
}

bool ChunkPlacer::RemoveStaleChunks(const std::unordered_set<Position>& currentChunkOrigins)
{
	for (auto chunksIterator = _loadedChunks.begin(); chunksIterator != _loadedChunks.end();)
	{
		const auto origin = chunksIterator->first;

		if (currentChunkOrigins.find(origin) == currentChunkOrigins.end())
		{
			const std::lock_guard<std::mutex> lock(_chunksMutex);

			auto handledChunk = std::move(_loadedChunks[origin]);
			chunksIterator = _loadedChunks.erase(chunksIterator);

			_freeChunks.emplace_back(std::move(handledChunk));
		}
		else
		{
			++chunksIterator;
		}
	}

	return false;
}

void ChunkPlacer::LazyLoader()
{
	_running = true;
	auto lastRememberedPosition = _previousNormalizedPosition - 1;

	while (_running)
	{
		if (!_hasPositionChanged)
		{
			std::unique_lock<std::mutex> loaderLock(_chunksMutex);
			_lazyLoaderLock.wait(loaderLock, [&lastRememberedPosition]
			{
				if (!_running)
				{
					return true;
				}

				if (lastRememberedPosition != _previousNormalizedPosition)
				{
					lastRememberedPosition = _previousNormalizedPosition;
					_hasPositionChanged = false;
					return true;
				}

				return false;
			});
		}
		else
		{
			_hasPositionChanged = false;
			lastRememberedPosition = _previousNormalizedPosition;
		}

		const auto currentChunksOrigins = _order->GetChunksAround(lastRememberedPosition);
		auto currentChunksOriginsSet = std::unordered_set<Position>(currentChunksOrigins.begin(), currentChunksOrigins.end());
		
		if (AddNewChunks(currentChunksOriginsSet)) continue;
		
		_hasPositionChanged = false;
	}
}

ChunkPlacer::ChunkPlacer(const OrderType orderType, const size_t chunkSize, const size_t renderDistance, const Position& initPosition)
{
	switch (orderType)
	{
	case OrderType::cube:	
		_order = std::make_unique<CubeOrder>(renderDistance, chunkSize);
		break;

	case OrderType::diamond: 
		_order = std::make_unique<DiamondOrder>(renderDistance, chunkSize);
		break;

	case OrderType::tiltedCube:	
		_order = std::make_unique<TiltedCubeOrder>(renderDistance, chunkSize);
		break;
	}

	_previousNormalizedPosition = GetNormalizedPosition(initPosition, chunkSize);
}

void ChunkPlacer::ReactToCameraMovement(const Position& position)
{
	const auto currentNormalizedPosition = GetNormalizedPosition(position, _order->GetChunkSize());

	if (currentNormalizedPosition != _previousNormalizedPosition)
	{
		_previousNormalizedPosition = currentNormalizedPosition;

		auto chunksPositionsAroundCameraVector = _order->GetChunksAround(_previousNormalizedPosition);
		_chunksPositionsAroundCamera = std::unordered_set<Position>(chunksPositionsAroundCameraVector.begin(), chunksPositionsAroundCameraVector.end());

		_hasPositionChanged = true;
		_lazyLoaderLock.notify_one();

		_log.Trace("Normalized position: " + PositionToString(currentNormalizedPosition));
	}
}

void ChunkPlacer::Bind(const std::shared_ptr<WorldGenerator>& generator, const size_t chunkSize)
{
	if (_generator != nullptr)
	{
		while (!_loadedChunks.empty())
		{
			_loadedChunks.erase(_loadedChunks.begin());
		}

		while (!_freeChunks.empty())
		{
			_freeChunks.pop_back();
		}
	}

	_generator = generator;

	const auto& chunksToGenerate = _order->GetChunksAmount();

	_freeChunks.reserve(chunksToGenerate);
	_loadedChunks.reserve(chunksToGenerate);

	for (size_t i = 0; i < chunksToGenerate; ++i)
	{
		_freeChunks.emplace_back(std::make_unique<Chunk>(chunkSize, _generator->GetBlockMap()));
	}

	auto chunksPositionsAroundCameraVector = _order->GetChunksAround(_previousNormalizedPosition);
	_chunksPositionsAroundCamera = std::unordered_set<Position>(chunksPositionsAroundCameraVector.begin(), chunksPositionsAroundCameraVector.end());

	_lazyLoader = std::make_unique<std::thread>(&LazyLoader);
}

std::mutex& ChunkPlacer::GetMutex()
{
	return _chunksMutex;
}

std::unordered_map<Position, std::unique_ptr<Chunk>>& ChunkPlacer::GetChunks() const
{
	if (!_chunksToLoad.empty())
	{
		const auto data = _chunksToLoad.back();

		if (_chunksPositionsAroundCamera.find(std::get<0>(data)) == _chunksPositionsAroundCamera.end() ||
			_loadedChunks.find(std::get<0>(data)) != _loadedChunks.end())
		{
			_chunksToLoad.pop_back();
		}
		else if (!_freeChunks.empty() && _chunksPositionsAroundCamera.find(std::get<0>(data)) != _chunksPositionsAroundCamera.end())
		{
			_chunksToLoad.pop_back();

			auto chunk = std::move(_freeChunks.back());
			_freeChunks.pop_back();

			const auto& origin = std::get<0>(data);
			const auto& blocks = std::get<1>(data);
			const auto& mesh = std::get<2>(data);

			chunk->LoadBlocks(blocks);
			chunk->LoadMesh(mesh);

			_loadedChunks[origin] = std::move(chunk);
		}
		else
		{
			auto chunksIterator = _loadedChunks.begin();
			do
			{
				const auto origin = chunksIterator->first;

				if (_chunksPositionsAroundCamera.find(origin) == _chunksPositionsAroundCamera.end())
				{
					auto handledChunk = std::move(_loadedChunks[origin]);
					_loadedChunks.erase(chunksIterator);

					_freeChunks.emplace_back(std::move(handledChunk));

					break;
				}
				
			} while (++chunksIterator != _loadedChunks.end());
		}
	}

	return _loadedChunks;
}

void ChunkPlacer::Terminate() const
{
	_running = false;
	_lazyLoaderLock.notify_one();
	_lazyLoader->join();

	_generator = nullptr;
}
