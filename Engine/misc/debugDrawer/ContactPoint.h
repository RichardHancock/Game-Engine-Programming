#pragma once

#include <glm/glm.hpp>

/** @brief	A contact point. */
struct ContactPoint
{
	/**
	 @brief	Constructor.
	
	 @param	inPoint 	The contact point.
	 @param	inNormal	The normal vector.
	 @param	inColour	The colour.
	 */
	ContactPoint(glm::vec3 inPoint, glm::vec3 inNormal, glm::vec3 inColour);

	/** @brief	The contact point. */
	glm::vec3 point;

	/** @brief	The normal vector. */
	glm::vec3 normal;

	/** @brief	The colour. */
	glm::vec3 colour;
};