#pragma once

#include <glm/glm.hpp>

/** @brief	A line. */
struct Line
{
	/**
	 @brief	Constructor.
	
	 @param	pointA  	The point a.
	 @param	pointB  	The point b.
	 @param	inColour	The colour.
	 */
	Line(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 inColour);

	/** @brief	Point A. */
	glm::vec3 pA;

	/** @brief	Point B. */
	glm::vec3 pB;

	/** @brief	The colour. */
	glm::vec3 colour;
};