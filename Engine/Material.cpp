#include "Material.h"

#include <GL/glew.h>

Material::Material(std::string vShaderFilename, std::string fShaderFilename)
{
	shader = std::make_shared<Shader>(vShaderFilename, fShaderFilename);
}

std::weak_ptr<Shader> Material::getShader()
{
	return shader;
}

void Material::addTexture(std::string name, std::weak_ptr<Texture> texture)
{
	if (texture.expired())
	{
		Log::logE("Texture passed to Material::addTexture is expired");
		return;
	}

	if (textures.count(name) == 1)
	{
		Log::logW(name + " already exists in texture array, this may cause undefined behaviours to occur");
	}

	GLint texID  = shader->getUniformLocation(name);
	if (texID == -1)
	{
		Log::logE(name + " could not be found in the shader uniforms");
		return;
	}

	textures[name] = GLTextureInfo(texture, texID);
}

void Material::useProgram()
{
	glUseProgram(shader->getProgram());
	
	//As the texture array is an unordered map a standard for loop is not possible, so 'i' must be counted externally
	GLuint currentTexUnit = 0;
	for (auto texture : textures)
	{
		glUniform1i(texture.second.index, currentTexUnit);
		
		///TODO Wrap in debug wrapper
		//Check for the maximum available texture units
		GLint maxAvailableTexUnits = 0;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxAvailableTexUnits);

		//Really unlikely but just to be safe
		assert((GLint)currentTexUnit < maxAvailableTexUnits);

		//Activate the next available texture unit
		glActiveTexture(GL_TEXTURE0 + currentTexUnit);
		//Provide the texture data's ID
		glBindTexture(GL_TEXTURE_2D, texture.second.data.lock()->getGLTexID());

		
		currentTexUnit++;
	}
	
}
