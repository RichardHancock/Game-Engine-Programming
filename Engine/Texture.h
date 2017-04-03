#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <GL/glew.h>

#include "misc/Vec2.h"
#include "Resource.h"

//Ref: Was used in previous assignment but with massive changes

/**
@class Texture
@brief Encapsulates an SDL_Texture

Contains the ability to draw, load, etc.. Textures.
*/
class Texture : public Resource
{
public:
	/**
	@brief Create a Texture
	@param path File path to image
	@param loadOpenGL Should the texture be copied into a texture buffer,
	*/
	Texture(std::string path, bool loadOpenGL = true);

	/**
	@brief Create a Texture
	@param surface A surface to convert into a texture (This is freed internally)
	@param loadOpenGL Should the texture be copied into a texture buffer,
	*/
	Texture(SDL_Surface* surface, bool loadOpenGL = true);

	/** @brief	Destructor. */
	~Texture();

	/**
	@brief Gets the Dimensions of the stored texture
	@return Vec2 - Dimensions of Sprite
	*/
	Vec2 getDimensions();


	/**
	@brief Load Image
	@param path File path to image
	*/
	bool load(std::string path);

	/**
	@brief Load Image
	@param image A surface to convert into a texture (This is freed internally)
	*/
	bool load(SDL_Surface* image);

	/**
	@brief Copies this Texture.
	
	@return null if it fails, else a SDL_Surface*.
	 */
	Texture* copy();

	/**
	 @brief Gets raw surface.
	
	 @return null if it fails, else the raw surface.
	 */
	SDL_Surface* getSurface() { return surface; }

	/**
	 @brief	Gets OpenGL texture identifier.
	
	 @return	The OpenGL texture identifier. -1 if not loaded
	 */
	GLuint getGLTexID();

	/**
	 @brief	Gets OpenGL texture format.
	
	 @param [in]	surface	If non-null, the surface.
	
	 @return	The OpenGL texture format.
	 */
	static GLenum getOpenGLTextureFormat(SDL_Surface* surface);

protected:

	/** @brief The surface. */
	SDL_Surface* surface;

	/** @brief	Loads for openGL. */
	void loadForOpenGL();

	/** @brief	Identifier for the texture in OpenGL. */
	GLuint texID;
};

