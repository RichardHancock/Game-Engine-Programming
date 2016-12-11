#include "Camera.h"

#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::~Camera()
{

}

glm::mat4 Camera::getProjMat()
{
	///TODO Ortho 
	
	return projMatrix;
}

void Camera::setProjMat(glm::mat4& newProjMatrix)
{
	projMatrix = newProjMatrix;
}

glm::mat4 Camera::getTransformMat()
{
	///TODO More error checking here
	return getGameObject().lock()->getComponent<Transform>("Transform").lock()->getTransformMat();
}

void Camera::onAwake()
{
	//Set to a basic Perspective matrix
	projMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 400.0f);
}

