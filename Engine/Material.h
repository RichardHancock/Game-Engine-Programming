#pragma once

#include <memory>
#include <unordered_map>

#include "Shader.h"
#include "Texture.h"
#include "CubeMap.h"

#include "misc/Log.h"

/** @brief	A material resource representing the lighting values. */
class Material : public Resource
{
private:
	/** @brief	Representation of a OpenGL texture. */
	struct GLTextureInfo
	{
		/** @brief	The texture data. */
		std::weak_ptr<Texture> data;

		/// @todo Make Texture and CubeMap share a base class to allow these to be merged

		/** @brief	The cube map. */
		std::weak_ptr<CubeMap> cubeMap;

		/** @brief	Zero-based index of the texture in OpenGL. */
		GLuint index;


		/** @brief	Type of the texture (ex. Cube map, Texture 2D). */
		GLenum textureType;

		/** @brief	Default constructor. */
		GLTextureInfo()
		{

		}

		/**
		 @brief	Constructor.
		
		 @param	data	   	The texture data.
		 @param	index	   	Zero-based index of the texture in OpenGL.
		 @param	textureType	Type of the texture.
		 */
		GLTextureInfo(std::weak_ptr<Texture> data, GLuint index, GLenum textureType)
		{
			this->data = data;
			this->index = index;
			this->textureType = textureType;
		}

		/**
		 @brief	Constructor.
		
		 @param	data	   	The cubeMap data.
		 @param	index	   	Zero-based index of the texture in OpenGL.
		 @param	textureType	Type of the texture.
		 */
		GLTextureInfo(std::weak_ptr<CubeMap> data, GLuint index, GLenum textureType)
		{
			this->cubeMap = data;
			this->index = index;
			this->textureType = textureType;
		}
	};

public:

	/**
	 @brief	Constructor.
	
	 @param	vShaderFilename	Filename of the shader file.
	 @param	fShaderFilename	Filename of the shader file.
	 */
	Material(std::string vShaderFilename, std::string fShaderFilename);

	/**
	 @brief	Gets the shader.
	
	 @return	The shader.
	 */
	std::weak_ptr<Shader> getShader();

	/**
	 @brief	Adds a texture to the named shader uniform.
	
	 @param	name   	The name.
	 @param	texture	The texture.
	 */
	void addTexture(std::string name, std::weak_ptr<Texture> texture);

	/**
	@brief	Adds a cube map to the named shader uniform.

	@param	name   	The name.
	@param	cubeMap	The cube map (Tends to be a skybox).
	*/
	void addCubeMap(std::string name, std::weak_ptr<CubeMap> cubeMap);

	/** @brief	Uses shader program. */
	void useProgram();

private:

	/** @brief	The shader. */
	std::shared_ptr<Shader> shader;

	/** @brief	The textures. */
	std::unordered_map<std::string, GLTextureInfo> textures;
};