#include "ChunkManager.h"

glm::vec3 ChunkManager::GetNormalizedPosition(glm::vec3 position) const
{
	position /= _chunkSize;
	position -= 0.5f;

	return {
		ceil(position.x),
		ceil(position.y),
		ceil(position.z)
	};
}

unsigned ChunkManager::CountChunksRecursive(const unsigned level)
{
	const auto result = 4 * level;

	if (result == 0) return 1;
	return result + CountChunksRecursive(level - 1);
}

void ChunkManager::UpdateChunksContainer(const glm::vec3 position)
{
	_loadedChunks.clear();

	for (auto y = -_renderDistance; y <= _renderDistance; ++y)
	{
		const auto xLimiter = _renderDistance - abs(y);
		for (auto x = -xLimiter; x <= xLimiter; ++x)
		{
			const auto zLimiter = abs(abs(x) + abs(y) - _renderDistance);
			for (auto z = -zLimiter; z <= zLimiter; ++z)
			{
				const auto origin = glm::vec3(x + static_cast<int>(position.x),
				                                     y + static_cast<int>(position.y),
				                                     z + static_cast<int>(position.z));

				_log.Trace("Updates chunk: " + 
						   std::to_string(origin.x) + ", " + 
						   std::to_string(origin.y) + ", " + 
						   std::to_string(origin.z));

				auto chunk = std::make_unique<Chunk>(origin, *this);

				auto chunkBlocks = _generator->GetPaintedChunkWithBorders(origin, _chunkSize);
				chunk->Load(chunkBlocks);

				_loadedChunks.push_back(std::move(chunk));
			}
		}
	}
}

ChunkManager::ChunkManager(const int chunkSize, const int renderDistance, Camera& camera)
	: _camera(camera), _renderDistance(renderDistance), _chunkSize(chunkSize), _generator(nullptr)
{
	_chunksToRender = GetChunksToRenderCount();
	_lastChunkWithPlayer = GetNormalizedPosition(_camera.GetPosition());

	UpdateChunksContainer(_lastChunkWithPlayer);
}

void ChunkManager::Update()
{
	const auto currentChunkWithPlayer = GetNormalizedPosition(_camera.GetPosition());
	if (currentChunkWithPlayer != _lastChunkWithPlayer)
	{
		_lastChunkWithPlayer = currentChunkWithPlayer;
		UpdateChunksContainer(_lastChunkWithPlayer);
	}
	
	for (const auto& chunk : _loadedChunks)
	{
		chunk->Draw();
	}
}

void ChunkManager::Bind(std::unique_ptr<WorldGenerator>& worldGenerator)
{
	if (!worldGenerator->IsInitialized())
	{
		throw std::runtime_error("The world generator is not initialized!");
	}

	_generator = std::move(worldGenerator);
}

unsigned ChunkManager::GetChunksToRenderCount() const
{
	unsigned result = 0;
	for (auto i = 0; i < _renderDistance; ++i)
	{
		result += 2 * CountChunksRecursive(i);
	}

	return CountChunksRecursive(_renderDistance) + result; 
}

Camera& ChunkManager::GetCamera() const
{
	return _camera;
}

unsigned ChunkManager::GetChunkSize() const
{
	return _chunkSize;
}
