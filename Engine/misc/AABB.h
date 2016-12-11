#pragma once

#include <glm/glm.hpp>

struct AABB
{
public:
	glm::vec3 center;
	glm::vec3 size;
	glm::vec3 halfSize;
	glm::vec3 min;
	glm::vec3 max;

	AABB();
	AABB(glm::vec3 center, glm::vec3 size);

	bool intersects(AABB other);

	float getRadius();
};