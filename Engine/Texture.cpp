#include "Texture.h"

#include <iostream>
#include <assert.h>

#include "misc/Log.h"
#include "Platform.h"

Texture::Texture(std::string filename, bool loadOpenGL)
	: Resource(), surface(nullptr), texID((GLuint)-1)
{	
	if (load(filename) && !Platform::isDummyRenderer() && loadOpenGL)
		loadForOpenGL();
}

Texture::Texture(SDL_Surface* surface, bool loadOpenGL)
	: Resource(), surface(nullptr), texID((GLuint)-1)
{
	if (load(surface) && !Platform::isDummyRenderer() && loadOpenGL)
		loadForOpenGL();

}

Texture::~Texture()
{
	SDL_FreeSurface(surface);

	if (texID != -1)
		glDeleteTextures(1, &texID);
}

bool Texture::load(std::string filename)
{
	Log::logI("Loading Image: " + filename);

	if (surface != nullptr)
	{
		Log::logE("Texture already loaded");
		assert(false);
		return false;
	}
	
	surface = IMG_Load(filename.c_str());
	if (!surface)
	{
		// Check if loaded successfully
		Log::logW("Can't load image named " + filename + " " + std::string(IMG_GetError()));
		return false;
	}

	return true;
}

bool Texture::load(SDL_Surface* image)
{
	if (surface != nullptr)
	{
		Log::logE("Texture already loaded");
		assert(false);
		return false;
	}

	surface = image;
	if (!image)
	{
		// Check if loaded successfully
		Log::logW("Image passed to Texture::load was null: " + std::string(IMG_GetError()));
		return false;
	}

	return true;
}

Texture* Texture::copy()
{
	SDL_PixelFormat* format = surface->format;

	SDL_Surface* tempSurface = SDL_CreateRGBSurface(
		0,
		surface->w,
		surface->h,
		format->BitsPerPixel,
		format->Rmask,
		format->Gmask,
		format->Bmask,
		format->Amask
		);
	
	assert(tempSurface != nullptr);

	//TODO: maybe should set blend modes to none

	SDL_BlitSurface(surface, NULL, tempSurface, NULL);

	return new Texture(tempSurface);
}

GLuint Texture::getGLTexID()
{
	return texID;
}

void Texture::loadForOpenGL()
{
	GLenum textureFormat = 0;

	//Section from http://ubuntuforums.org/archive/index.php/t-1088836.html post 2
#ifdef _MSC_VER
	__pragma(warning(push));
	__pragma(warning(disable:4127)); // This is a problem in SDL2 (Can't fix) and isn't any danger
#endif // _MSC_VER

	switch (surface->format->BytesPerPixel) {
	case 4:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGRA;
		else
			textureFormat = GL_RGBA;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGR;
		else
			textureFormat = GL_RGB;
		break;
	}

#ifdef _MSC_VER
	__pragma(warning(pop));
#endif // _MSC_VER
	//End Section

	assert(textureFormat != 0);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		textureFormat,
		surface->w,
		surface->h,
		0,
		textureFormat,
		GL_UNSIGNED_BYTE,
		surface->pixels
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}


Vec2 Texture::getDimensions()
{
	//Returns a Vec2 of the dimension unless surface is nullptr in which case a 0x0 Vec2 is returned
	return (surface != nullptr ? Vec2((float) surface->w, (float) surface->h) : Vec2(0.0f));
}
