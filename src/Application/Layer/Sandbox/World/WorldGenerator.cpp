#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(const int seed) : _seed(seed)
{
	auto biomeProvider = BiomeProvider("src/Data/Generators.json", _blockMap);
	_biomes = biomeProvider.GetBiomes(_seed);

	const auto placerNoise = biomeProvider.GetPlacerNoise(_seed);
	_placer = std::make_unique<BiomePlacer>(placerNoise, _biomes);
}

void WorldGenerator::PaintChunk(const ChunkFrame& frame, ChunkBlocks& blocks) const
{
	_placer->PaintChunk(frame, blocks);
}

BlockMap& WorldGenerator::GetBlockMap()
{
	return _blockMap;
}
