#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"

Transform::Transform() :
	Component()
{
	matrix = glm::mat4(1);
}

Transform::Transform(glm::mat4 &matrix) :
	Component(), matrix(matrix)
{

}

glm::mat4 Transform::translate(glm::vec3 translation)
{
	return matrix = glm::translate(matrix, translation);
}

glm::mat4 Transform::rotate(glm::vec3 rotation)
{
	if (rotation.x != 0.00f)
	{
		matrix = glm::rotate(matrix, rotation.x, glm::vec3(1, 0, 0));
	}
	if (rotation.y != 0.00f)
	{
		matrix = glm::rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
	}
	if (rotation.z != 0.00f)
	{
		matrix = glm::rotate(matrix, rotation.z, glm::vec3(0, 0, 1));
	}

	return matrix;
}

glm::mat4 Transform::scale(glm::vec3 scale)
{
	return matrix = glm::scale(matrix, scale);
}

glm::mat4 Transform::getTransform()
{
	return matrix;
}


