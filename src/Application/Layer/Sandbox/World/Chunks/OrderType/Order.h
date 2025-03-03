#pragma once
#include <vector>
#include <glm/vec3.hpp>

/// @class Order
///	@brief The base class for creating placing order types.
///	@details An order type is a class providing an algorithm, by which
///	the chunks are placer by the ChunkPlacer.
class Order  // NOLINT(cppcoreguidelines-special-member-functions)
{
	size_t _renderDistance;
	size_t _chunkSize;

public:

	/// @brief The constructor.
	///	@param renderDistance - the maximum distance from the camera to render.
	///	@param chunkSize - the size of the generated chunks.
	Order(const size_t& renderDistance, const size_t& chunkSize)
		: _renderDistance(renderDistance), _chunkSize(chunkSize)
	{}

	/// @brief Sets render distance.
	///	@param renderDistance - the maximum distance from the camera to render.
	void SetRenderDistance(const size_t& renderDistance)
	{
		_renderDistance = renderDistance;
	}

	/// @brief Returns render distance.
	size_t GetRenderDistance() const
	{
		return _renderDistance;
	}

	/// @brief Sets chunk size.
	///	@param chunkSize - the size of the generated chunks.
	void SetChunkSize(const size_t& chunkSize)
	{
		_chunkSize = chunkSize;
	}

	/// @brief Returns chunk size.
	size_t GetChunkSize() const
	{
		return _chunkSize;
	}

	/// @brief Returns the chunks amount that are rendered within this render view.
	virtual size_t GetChunksAmount() = 0;


	/// @brief Get the amount of chunks that could be generated on the current render distance settings.
	virtual std::vector<glm::ivec3> GetChunksAround(glm::ivec3 normalizedOrigin) = 0;

	virtual ~Order() = default;
};
