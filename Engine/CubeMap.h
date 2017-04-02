#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL.h>

class CubeMap
{
public:
	CubeMap(
		std::string rightFacePath,
		std::string leftFacePath,
		std::string topFacePath,
		std::string bottomFacePath,
		std::string backFacePath,
		std::string frontFacePath
	);


	/**
	@brief	Gets OpenGL texture identifier.

	@return	The OpenGL texture identifier. -1 if not loaded
	*/
	GLuint getGLTexID();

protected:

	SDL_Surface* loadSurface(std::string path);

	/** @brief	Identifier for the texture in OpenGL. */
	GLuint texID;

	static const int SIDES_OF_CUBE = 6;
};