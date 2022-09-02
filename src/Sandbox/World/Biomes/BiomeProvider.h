#pragma once
#include "Biome.h"
#include "Core/Metadata.h"
#include "Sandbox/Utils/ProceduralGen/Noise2D.h"

/// @class BiomeProvider
/// @brief Provides the biomes specified in the JSON file.
///	@details This class manages creating the list of biomes that are specified in the JSON file.
///	It's purpose is to provide all the defined biomes from the file in a convenient way.
class BiomeProvider
{
	Metadata _biomesMetadata;
	Shader& _blockShader;

public:

	/// @brief The constructor.
	///	@param filenameWithBiomeData - Filename that contains all the metadata related to biomes.
	///	@param blockShader - A reference to the block shader which is used by the @ref Biome class.
	explicit BiomeProvider(const std::string& filenameWithBiomeData, Shader& blockShader);

	/// @brief Get noise that is used by placer that represents world map.
	///	@param seed - a seed of the world map noise.
	///	@param biomesType - type of biomes that we want to apply, to the world.
	Noise2D GetPlacerNoise(int seed, const std::string& biomesType = "default");

	/// @brief Get all the biomes specified in the file with a particular type.
	///	@param seed - a seed of the biome noises.
	///	@param biomesType - type of biomes that we want to apply, to the world.
	std::vector<Biome> GetBiomes(int seed, const std::string& biomesType = "default");
};

