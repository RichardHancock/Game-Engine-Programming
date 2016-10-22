#pragma once

#include <memory>

#include "Component.h"
#include "misc/Vec3.h"
#include "glm/matrix.hpp"

class Transform : public Component
{
public:
	
	virtual ~Transform();

	glm::vec3 getPostion();
	glm::vec3 getRotation();
	
	void setPostion(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	
	glm::mat4 translate(glm::vec3 translation);

	glm::mat4 rotate(glm::vec3 rotation);

	glm::mat4 scale(glm::vec3 scale);

	glm::mat4 getTransform();

private:

	glm::vec3 localPosition;
	glm::vec3 localRotation;
	glm::vec3 localScale;
	std::weak_ptr<Transform> parent;
	std::vector<std::weak_ptr<Transform>> children;

};