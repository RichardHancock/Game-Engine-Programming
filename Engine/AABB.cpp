#include "AABB.h"

AABB::AABB()
{
	size = glm::vec3(0);
	center = glm::vec3(0);
	halfSize = glm::vec3(0);
	min = glm::vec3(0);
	max = glm::vec3(0);
}

AABB::AABB(glm::vec3 center, glm::vec3 size)
	: center(center), size(size)
{
	halfSize = size / 2.0f;
	min = center - halfSize;
	max = center + halfSize;
}

bool AABB::intersects(AABB other)
{
	bool collision[3];

	//Loop through all axis (I rolled it into a loop for hopefully better performance)
	for (unsigned int axis = 0; axis < 3; axis++)
	{
		if (min[axis] < other.min[axis])
		{
			if (max[axis] >= other.min[axis])
			{
				collision[axis] = true;
			}
		}
		else if (other.max[axis] >= min[axis])
		{
			collision[axis] = true;
		}
	}


	//Return true if all collision axis are true;
	return (collision[0] && collision[1] && collision[2]);
}

float AABB::getRadius()
{
	return halfSize.x;
}
