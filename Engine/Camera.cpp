#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "misc/Utility.h"

Camera::Camera()
{
	// Create a viewing matrix for the camera
	viewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, -1.0f, -2.5f));
	viewMatrix = glm::rotate(viewMatrix, Utility::HALF_PI, glm::vec3(0, 1, 0));

	// Construct a projection matrix for the camera
	projMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 200.0f);

	// Initialise the orientation
	orientation = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::lookAt(glm::vec3 targetPos)
{
	glm::vec3 upDir(0, 1, 0);
	glm::vec3 cameraPos = targetPos;
	cameraPos.z += 2.0f;

	targetPos.z -= 2.0f;

	viewMatrix = glm::lookAt(cameraPos, targetPos, upDir);
}

void Camera::moveCamera(glm::vec3 translation)
{
	/*Create a viewing matrix for the camera 'Camera orientation'.
	The translation is inverted to work with OpenGL*/
	viewMatrix = glm::translate(viewMatrix, -translation);
}

void Camera::moveCameraAlongX(float translateX)
{
	/*Create a viewing matrix for the camera 'Camera orientation'.
	The translation is inverted to work with OpenGL*/
	viewMatrix = glm::translate(viewMatrix, glm::vec3(-translateX, 0.0f, 0.0f));
}

void Camera::moveCameraAlongY(float translateY)
{
	/*Create a viewing matrix for the camera 'Camera orientation'.
	The translation is inverted to work with OpenGL*/
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, -translateY, 0.0f));
}

void Camera::moveCameraAlongZ(float translateZ)
{
	/*Create a viewing matrix for the camera 'Camera orientation'.
	The translation is inverted to work with OpenGL*/
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -translateZ));
}

void Camera::rotateCamera(glm::vec3 rotation)
{
	//Rotates the Camera
	viewMatrix = glm::rotate(viewMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	
	//update the stored orientation
	orientation = rotation;
}

void Camera::rotateCameraAlongX(float rotationX)
{
	//Rotates the Camera along the X axis
	viewMatrix = glm::rotate(viewMatrix, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
	
	//update the stored orientation
	orientation.x = rotationX;
}

void Camera::rotateCameraAlongY(float rotationY)
{
	//Rotates the Camera along the Y axis
	viewMatrix = glm::rotate(viewMatrix, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
	
	//update the stored orientation
	orientation.y = rotationY;
}

void Camera::rotateCameraAlongZ(float rotationZ)
{
	//Rotates the Camera along the Z axis
	viewMatrix = glm::rotate(viewMatrix, rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
	
	//update the stored orientation
	orientation.z = rotationZ;
}

/*
void Camera::updateViewMat(glm::vec3 targetPos)
{
	glm::vec3 upDir(0, 1, 0);
	glm::vec3 cameraPos = targetPos;
	//cameraPos.y += 1.25f;
	cameraPos.z += 2.0f;

	targetPos.z -= 2.0f;

	viewMatrix = glm::lookAt(cameraPos, targetPos, upDir);

	//viewMatrix = glm::rotate(viewMatrix, Utility::HALF_PI, glm::vec3(0, 1, 0));
}*/