#include "Material.h"

void Material::addTexture(std::string name, std::weak_ptr<Texture> texture)
{
	if (texture.expired)
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
