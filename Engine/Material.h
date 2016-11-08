#pragma once

#include <memory>
#include <unordered_map>

#include "Shader.h"
#include "Texture.h"

#include "misc/Log.h"

class Material
{
private:
	struct GLTextureInfo
	{
		std::weak_ptr<Texture> data;
		GLuint index;

		GLTextureInfo(std::weak_ptr<Texture> data, GLuint index)
			: data(data), index(index)
		{

		}
	};

public:

	Material(std::string vShaderFilename, std::string fShaderFilename);

	std::weak_ptr<Shader> getShader();

	void addTexture(std::string name, std::weak_ptr<Texture> texture);

private:


	std::shared_ptr<Shader> shader;

	std::unordered_map<std::string, GLTextureInfo> textures;


	///TODO Remember to check available texture units: GL_MAX_TEXTURE_UNITS
};