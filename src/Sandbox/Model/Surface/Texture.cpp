#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Texture::LoadTexture(const unsigned char* image, const int& width, const int& height)
{
	glGenTextures(1, &_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(_textureType, _texture);

	glTexParameteri(_textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(_textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(_textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(_textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(_textureType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(_textureType);

	glBindTexture(_textureType, 0);
}

Texture::Texture(const std::string& filenameWithImage)
{
	auto width = 0;
	auto height = 0;
	auto channelsInFile = 0;

	stbi_set_flip_vertically_on_load(true);
	const auto imageData = stbi_load(filenameWithImage.c_str(), &width, &height, &channelsInFile, 0);

	LoadTexture(imageData, width, height);

	stbi_image_free(imageData);
}

Texture::Texture(const std::string& filenameWithImage, const int x, const int y, const size_t spriteSize)
{
	auto width = 0;
	auto height = 0;
	auto channelsInFile = 0;

	stbi_set_flip_vertically_on_load(true);
	const auto imageData = stbi_load(filenameWithImage.c_str(), &width, &height, &channelsInFile, 0);

	const auto invertedY = height / spriteSize - y - 1;

	_textureAtlasCoords = 
	{{
		{
			static_cast<float>(x) * static_cast<float>(spriteSize) / static_cast<float>(width),
			static_cast<float>(invertedY) * static_cast<float>(spriteSize) / static_cast<float>(height)
		},
		{
			(static_cast<float>(x) + 1.0f) * static_cast<float>(spriteSize) / static_cast<float>(width),
			static_cast<float>(invertedY) * static_cast<float>(spriteSize) / static_cast<float>(height)
		},
		{
			(static_cast<float>(x) + 1.0f) * static_cast<float>(spriteSize) / static_cast<float>(width),
			(static_cast<float>(invertedY) + 1.0f) * static_cast<float>(spriteSize) / static_cast<float>(height)
		},
		{
			static_cast<float>(x) * static_cast<float>(spriteSize) / static_cast<float>(width),
			(static_cast<float>(invertedY) + 1.0f) * static_cast<float>(spriteSize) / static_cast<float>(height)
		}
	}};

	LoadTexture(imageData, width, height);

	stbi_image_free(imageData);
}

void Texture::SetUvToTextureAtlas(std::vector<Vertex>& vertices) const
{
	constexpr auto verticesAmount = 4;
	for (auto i = 0; i < verticesAmount; ++i)
	{
		vertices.at(i).uvCoordinate = _textureAtlasCoords[i];
	}
}

void Texture::Bind(const Shader& shader) const
{
	const auto textureUniform = glGetUniformLocation(shader.GetProgram(), "textureSample");

	shader.Load();
	glUniform1i(textureUniform, 0);

	glBindTexture(_textureType, _texture);
}

void Texture::Unbind() const
{
	glBindTexture(_textureType, 0);
}

Texture::~Texture()
{
	glBindTexture(_textureType, 0);
	glDeleteTextures(1, &_texture);
}
