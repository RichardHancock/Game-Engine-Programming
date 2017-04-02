#include "CubeMap.h"

#include <assert.h>
#include <SDL_image.h>
#include <vector>

#include "misc/Log.h"
#include "Texture.h"

CubeMap::CubeMap(std::string rightFacePath, std::string leftFacePath, std::string topFacePath, std::string bottomFacePath, std::string backFacePath, std::string frontFacePath)
{
	std::vector<SDL_Surface*> surfaces;
	surfaces.reserve(SIDES_OF_CUBE);

	surfaces.push_back(loadSurface(rightFacePath));
	surfaces.push_back(loadSurface(leftFacePath));
	surfaces.push_back(loadSurface(topFacePath));
	surfaces.push_back(loadSurface(bottomFacePath));
	surfaces.push_back(loadSurface(backFacePath));
	surfaces.push_back(loadSurface(frontFacePath));

	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	for (unsigned int sideIndex = 0; sideIndex < SIDES_OF_CUBE; sideIndex++)
	{
		SDL_Surface* surface = surfaces[sideIndex];
		assert(surface != nullptr);

		GLenum textureFormat = Texture::getOpenGLTextureFormat(surface);
		assert(textureFormat != 0);

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + sideIndex,
			0,
			textureFormat,
			surface->w,
			surface->h,
			0,
			textureFormat,
			GL_UNSIGNED_BYTE,
			surface->pixels
		);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

GLuint CubeMap::getGLTexID()
{
	return texID;
}

SDL_Surface * CubeMap::loadSurface(std::string path)
{
	SDL_Surface* surface = nullptr;

	Log::logI("Loading CubeMap Image: " + path);

	surface = IMG_Load(path.c_str());
	if (!surface)
	{
		// Check if loaded successfully
		Log::logW("Can't load image named " + path + " " + std::string(IMG_GetError()));
		return nullptr;
	}

	return surface;
}
