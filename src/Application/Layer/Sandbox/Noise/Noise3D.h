#pragma once
#include <vector>

#include "Noise.h"
#include "Application/Layer/Sandbox/Model/Mesh/Geometry/Structures.h"
#include "Application/Layer/Sandbox/World/Chunks/Structure/ChunkFrame.h"


/// @class Noise3D
/// @brief Used for gathering data from the defined 3D noise.
class Noise3D : protected Noise
{
	static glm::ivec3 GetOriginShiftedByExpansionFactor(const ChunkFrame& frame, int xOffset, int yOffset, int zOffset, int expansionFactor);
	static std::vector<std::vector<std::vector<float>>> ConvertNoiseFrom1DTo3D(const std::vector<float>& noise, const size_t& size);

public:

	/// @brief The constructor.
	///	@param encodedTree - A string with encoded tree generated by using FastNoise2 tool.
	///	@param seed - noise seed for different results.
	///	@param frequency - The frequency of the encoded noise function.
	Noise3D(const std::string& encodedTree, const int seed, const float frequency)
		: Noise(encodedTree, seed, frequency)
	{
	}

	/// @brief Get noise that is at certain point of the chunk.
	/// @param frame - the frame of the chunk.
	/// @param xOffset - offset from the origin X axis. 
	///	@param yOffset - offset from the origin Y axis. 
	///	@param zOffset - offset from the origin Z axis. 
	float GetNoiseAt(const ChunkFrame& frame, int xOffset, int yOffset, int zOffset) const;

	std::vector<float> GetNoiseAtWithTopColumn(const Position& origin, const size_t& topColumnHeight) const;

	/// @brief Get noise of the chunk column.
	/// @param frame - the frame of the chunk.
	/// @param xOffset - offset from the origin X axis. 
	///	@param yOffset - offset from the origin Y axis.
	///	@param zOffset - offset from the origin Z axis. 
	/// @param expansionFactor - the factor that expands or shrinks the dimensions of the chunk.
	std::vector<float> GetColumnNoise(const ChunkFrame& frame, int xOffset, int yOffset, int zOffset, int expansionFactor = 0) const;

	/// @brief Get noise of the chunk at certain position on the map.
	/// @param frame - frame of the chunk.
	/// @param xOffset - offset from the origin X axis. 
	///	@param yOffset - offset from the origin Y axis.
	///	@param zOffset - offset from the origin Z axis. 
	/// @param expansionFactor - the factor that expands or shrinks the dimensions of the chunk.
	std::vector<std::vector<std::vector<float>>> GetNoise(const ChunkFrame& frame, int xOffset, int yOffset, int zOffset, int expansionFactor = 0) const;

	/// @brief Get noise of the chunk at certain position on the map.
	/// @param frame - frame of the chunk.
	/// @param expansionFactor - the factor that expands or shrinks the dimensions of the chunk.
	std::vector<std::vector<std::vector<float>>> GetNoise(const ChunkFrame& frame, int expansionFactor = 0) const;
};
