#pragma once
#define FASTNOISE_STATIC_LIB 1

#include <FastNoise/FastNoise.h>
#include <string>

// ReSharper disable once CppUnusedIncludeDirective
#include "Sandbox/Utils/EngineExceptions.h"

/// @class Noise
/// @brief Represents the base class for the classes dedicated to generate noise algorithm.
class Noise
{
	FastNoise::SmartNode<> _noiseGenerator;

	const int _seed;
	const float _frequency;

protected:

	/// @brief The constructor.
	///	@param encodedTree - A string with encoded tree generated by using FastNoise2 tool.
	///	@param seed - noise seed for different results.
	///	@param frequency - The frequency of the encoded noise function.
	Noise(const std::string& encodedTree, const int seed, const float frequency)
		: _seed(seed), _frequency(frequency)
	{
		_noiseGenerator = FastNoise::NewFromEncodedNodeTree(encodedTree.c_str());
	}

	/// @brief Get noise generator.
	FastNoise::SmartNode<> GetGenerator() const
	{
		return _noiseGenerator;
	}

	/// @brief Get noise generator.
	int GetSeed() const
	{
		return _seed;
	}

	/// @brief Get noise generator.
	float GetFrequency() const
	{
		return _frequency;
	}
};

