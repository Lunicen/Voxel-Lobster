#pragma once
#include "RenderView.h"

/// @class TiltedCube
///	@brief Provides an algorithm that the renderer renders chunks in as a tilted cube.
/// @details A tilted cube is just a cube that it's edges are pointing cardinal directions.
class TiltedCube final : public RenderView
{
public:

	/// @brief The constructor.
	///	@param renderDistance - the maximum distance from the camera to render.
	///	@param chunkSize - the size of the generated chunks.
	TiltedCube(const size_t& renderDistance, const size_t& chunkSize)
		: RenderView(renderDistance, chunkSize)
	{
	}

	size_t GetChunksAmount() override;
	std::vector<glm::ivec3> GetChunksAround(glm::ivec3 normalizedOrigin) override;
};

