#pragma once

#include "Component.h"

#include <glm/glm.hpp>

class Camera : public Component
{
public:
	
	virtual ~Camera();

	glm::mat4 getProjMat();

	void setProjMat(glm::mat4& newProjMatrix);

	glm::mat4 getTransformMat();

	virtual void onAwake();

private:

	glm::mat4 projMatrix;
};