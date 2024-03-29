#pragma once

#include <stdint.h>
#include <SDL.h>
#include <glm/glm.hpp>

#include "Vec3.h"

//Ref: Was used in previous assignment

struct Colour
{
	///Red Channel
	uint8_t r;

	///Green Channel
	uint8_t g;

	///Blue Channel
	uint8_t b;

	///Alpha Channel
	uint8_t a;

	Colour();

	Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	Colour(uint8_t r, uint8_t g, uint8_t b);

	Colour(uint8_t rgb);

	Colour(Vec3 colour);

	/**
	@brief Convert to SDL colour.

	@return A SDL_Colour.
	*/
	SDL_Colour convertToSDLColour();

	///TO Document
	glm::vec4 convertToNormalizedColour();
};