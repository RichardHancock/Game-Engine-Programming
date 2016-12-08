#include "AABB.h"

AABB::AABB()
{
	AABB::AABB(glm::vec3(0.0f), glm::vec3(0.0f));
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
