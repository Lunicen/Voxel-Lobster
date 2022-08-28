#include "BiomeProvider.h"

#include "Sandbox/Utils/EngineExceptions.h"

void BiomeProvider::CheckIfDataIsLoaded() const
{
	if (!_biomesMetadata.IsLoaded())
	{
		throw UninitializedPropertyAccessException("Json file with biomes data is not loaded!");
	}
}

BiomeProvider::BiomeProvider(const std::string& filenameWithBiomeData, Shader& blockShader) : _blockShader(blockShader) 
{
	_biomesMetadata.Load(filenameWithBiomeData);
}

Noise2D BiomeProvider::GetPlacerNoise(const int seed, const std::string& biomesType)
{
	CheckIfDataIsLoaded();

	auto pattern = _biomesMetadata.GetJsonObject(biomesType)["pattern"];

	const auto id = pattern["id"].get<std::string>();
	const auto frequency = pattern["frequency"].get<float>();

	return {id, seed, frequency};
}

std::vector<Biome> BiomeProvider::GetBiomes(const int seed, const std::string& biomesType)
{
	CheckIfDataIsLoaded();

	auto biomesPool = _biomesMetadata.GetJsonObject(biomesType)["biomes"];
	auto biomes = std::vector<Biome>();

	for (auto& biome : biomesPool)
	{
		const auto name = biome["name"].get<std::string>();

		const auto id = biome["noise"]["id"].get<std::string>();
		const auto frequency = biome["noise"]["frequency"].get<float>();

		biomes.emplace_back(name, Noise3D(id, seed, frequency), _blockShader);
	}

	return biomes;
}
