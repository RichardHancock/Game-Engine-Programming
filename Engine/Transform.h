#pragma once

#include "Component.h"
#include "misc/Vec3.h"
#include "glm/matrix.hpp"

class Transform : public Component
{
public:
	
	Transform();

	Transform(glm::mat4& matrix);

	
	glm::mat4 translate(glm::vec3 translation);

	glm::mat4 rotate(glm::vec3 rotation);

	glm::mat4 scale(glm::vec3 scale);

	glm::mat4 getTransform();

private:

	glm::mat4 matrix;
};