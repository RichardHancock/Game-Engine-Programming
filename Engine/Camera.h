#pragma once

#include "Component.h"

#include <glm/glm.hpp>

class Camera : public Component
{
public:
	
	virtual ~Camera();

	glm::mat4 getProjMat();

	void setProjMat(glm::mat4& projMatrix);

	glm::mat4 getTransformMat();

private:


};