#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL.h>

/** @brief	A cube map. */
class CubeMap
{
public:

	/**
	 @brief	Constructor.
	
	 @param	rightFacePath 	Full pathname of the right face texture.
	 @param	leftFacePath  	Full pathname of the left face texture.
	 @param	topFacePath   	Full pathname of the top face texture.
	 @param	bottomFacePath	Full pathname of the bottom face texture.
	 @param	backFacePath  	Full pathname of the back face texture.
	 @param	frontFacePath 	Full pathname of the front face texture.
	 */
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

	/**
	 @brief	Loads a surface.
	
	 @param	path	Full pathname of the file.
	
	 @return	Null if it fails, else the surface.
	 */
	SDL_Surface* loadSurface(std::string path);

	/** @brief	Identifier for the texture in OpenGL. */
	GLuint texID;

	/** @brief	The sides of cube. */
	static const int SIDES_OF_CUBE = 6;
};