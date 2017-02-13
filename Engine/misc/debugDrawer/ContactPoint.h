#pragma once

#include <glm/glm.hpp>

struct ContactPoint
{
	ContactPoint(glm::vec3 inPoint, glm::vec3 inNormal, glm::vec3 inColour);

	glm::vec3 point;

	glm::vec3 normal;

	glm::vec3 colour;
};