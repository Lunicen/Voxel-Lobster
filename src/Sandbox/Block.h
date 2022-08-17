#pragma once
#include "Utils/Shader.h"
#include "Utils/ElementBuffer.h"
#include "Utils/VertexArray.h"
#include <glm/gtx/transform.hpp>
#include <stb_image.h>



/// @class Block
/// @brief Handles Block existence
class Block
{
	//    5-------6  
	//   /|      /|   
	//  1-------2 | 
	//  | |     | |   
	//  | 4-----|-7  
	//  |/      |/    
	//  0-------3


#pragma region Wersja z teksturami
	GLfloat _vertices[48] =
	{
		// Coordinates			//texture coordinates
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,
	};
#pragma endregion
	/*
	GLfloat _vertices[24] =
	{
		// Coordinates			//texture coordinates
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	};
	*/

	GLuint _indices[36]
	{
		// bottom
		0, 4, 7,
		7, 3, 0,

		0, 3, 2,

		// top
		2, 6, 5,
		5, 1, 2,

		2, 3, 7,
		7, 4, 5,
		5, 4, 0,

		// reverse
		0, 1, 5,
		5, 6, 7,
		7, 6, 2,
		2, 1, 0
	};

	Shader& _shader;
	glm::mat4 _position{};

	VertexArray _vao;
	VertexBuffer _vbo = VertexBuffer(_vertices, sizeof _vertices);
	ElementBuffer _ebo = ElementBuffer(_indices, sizeof _indices);
	//TODO tutaj chyba trzeba bedzie dodac dodatkowe zlinkowanie vbo... link/LinkAttrib

#pragma region Texture
	
	int widthImage;
	int heightImage;
	int numColCh; //TODO ??? sprawdzic co to jest
	unsigned char* bytes = stbi_load("./src/Data/Textures/Dirt.png", &widthImage, &heightImage, &numColCh, 0);
	GLuint texture;
	
#pragma endregion

public:
	/// @brief This is a Constructor that allows to create a block.
	/// @param x - X coordinate.
	///	@param y - Y coordinate.
	///	@param z - Z coordinate.
	///	@param shader - Shader that block will use.
	Block(float x, float y, float z, Shader& shader);

	/// @brief This moves block to the x, y, z coordinates.
	/// @param x - X coordinate.
	///	@param y - Y coordinate.
	///	@param z - Z coordinate.
	void Move(float x, float y, float z);

	/// @brief This draws block.
	void Draw() const;

	/// @brief This returns shader that block is using.
	Shader GetShader() const;
};

