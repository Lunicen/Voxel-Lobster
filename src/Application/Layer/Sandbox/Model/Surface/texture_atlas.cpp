#include "application/layer/sandbox/model/surface/texture_atlas.h"

#include <array>

std::array<Point, VerticesInQuad> TextureAtlas::GetTextureCoordinatesAt(const unsigned x, const unsigned y)
{
	const auto& width = GetWidth();
	const auto& height = GetHeight();

	const auto invertedY = height / _spriteSize - y - 1;

	const std::array<Point, VerticesInQuad> textureCoords = 
	{{
		{
			static_cast<float>(x) * static_cast<float>(_spriteSize) / static_cast<float>(width),
			static_cast<float>(invertedY) * static_cast<float>(_spriteSize) / static_cast<float>(height)
		},
		{
			(static_cast<float>(x) + 1.0f) * static_cast<float>(_spriteSize) / static_cast<float>(width),
			static_cast<float>(invertedY) * static_cast<float>(_spriteSize) / static_cast<float>(height)
		},
		{
			(static_cast<float>(x) + 1.0f) * static_cast<float>(_spriteSize) / static_cast<float>(width),
			(static_cast<float>(invertedY) + 1.0f) * static_cast<float>(_spriteSize) / static_cast<float>(height)
		},
		{
			static_cast<float>(x) * static_cast<float>(_spriteSize) / static_cast<float>(width),
			(static_cast<float>(invertedY) + 1.0f) * static_cast<float>(_spriteSize) / static_cast<float>(height)
		}
	}};

	return textureCoords;
}

TextureAtlas::TextureAtlas(const std::string& filenameWithImage, const size_t spriteSize) : Texture(filenameWithImage), _spriteSize(spriteSize)
{
}

void TextureAtlas::SetSprite(std::vector<Vertex>& vertices, const unsigned x, const unsigned y, const bool flip)
{
	const auto& textureCoords = GetTextureCoordinatesAt(x, y); 

	for (size_t i = 0; i < VerticesInQuad; ++i)
	{
		if (flip)
		{
			vertices.at((i + VerticesInQuad - 1) % VerticesInQuad).uvCoordinate = textureCoords[i];
		}
		else
		{
			vertices.at(i).uvCoordinate = textureCoords[i];
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void TextureAtlas::SetSprite(std::vector<Vertex>& vertices, const std::array<Point, VerticesInQuad>& textureUvCoordinates) const
{
	for (size_t i = 0; i < VerticesInQuad; ++i)
	{
		vertices.at(i).uvCoordinate = textureUvCoordinates[i];
	}
}


void TextureAtlas::SetSprite(std::array<Point, VerticesInQuad>& textureUvCoordinates, const unsigned x, const unsigned y, const bool flip)
{
	const auto& textureCoords = GetTextureCoordinatesAt(x, y); 

	if (!flip)
	{
		textureUvCoordinates = textureCoords;
		return;
	}
	
	for (size_t i = 0; i < VerticesInQuad; ++i)
	{
		textureUvCoordinates[(i + VerticesInQuad - 1) % VerticesInQuad] = textureCoords[i];
	}
}
