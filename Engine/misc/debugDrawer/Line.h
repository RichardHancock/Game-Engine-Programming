#pragma once

#include <glm/glm.hpp>

struct Line
{
	Line(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 inColour);

	glm::vec3 pA;

	glm::vec3 pB;

	glm::vec3 colour;
};