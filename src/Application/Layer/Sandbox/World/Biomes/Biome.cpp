#include "Biome.h"

#include "Application/Layer/Sandbox/World/Chunks/ChunkUtils.h"

void Biome::SetBlockAccordingToNoise(const glm::ivec3& origin, ChunkBlocks& blocks, const size_t& chunkSize, const std::vector<float>& noise, const size_t yLevel, const Byte& visibilityFlags) const
{
	auto blockIndex = _depthLevel.size() - 1;
	while (blockIndex > 0)
	{
		const auto& noiseOverThisBlock = yLevel + _depthLevel[blockIndex].first;
		if (noise[noiseOverThisBlock] <= 0)
		{
			break;
		}

		--blockIndex;
	}

	Byte blockFlags = visibilityFlags;
	blockFlags |= 0b00000010; // Enables block, so it's not treated as an air.

	const BlockData blockData = 
	{
		_blocksMap.GetId(_depthLevel[blockIndex].second),
		blockFlags
	};

	blocks[ChunkUtils::GetBlockIndex(origin, chunkSize)] = BlockData(blockData);
}

Biome::Biome(std::string name, const Noise3D& noise, std::vector<std::pair<size_t, std::string>> depthLevels, BlockMap& blocksMap)
	: Noise3D(noise), _name(std::move(name)), _depthLevel(std::move(depthLevels)), _blocksMap(blocksMap)
{
}

void Biome::PaintBlockAt(const Position& origin, const ChunkFrame& frame, ChunkBlocks& blocks, const Byte& visibilityFlags) const
{
	const auto noise = GetColumnNoiseWithAdditionalHeight(
		frame,
		origin.x, 
		origin.y, 
		origin.z, 
		_depthLevel.back().first
	);

	SetBlockAccordingToNoise(
		origin,
		blocks,
		frame.size,
		noise, origin.y,
		visibilityFlags
	);
}
