#pragma once

#include <glm/glm.hpp>

/** @brief A camera that holds and manipulates the View and Projection Matrices. */
class Camera
{
public:
	/** 
	@brief Default constructor. 
	*/
	Camera();

	/**
	@brief Updates the view matrix to look at targetPos.
	@param targetPos Target position.
	*/
	void lookAt(glm::vec3 targetPos);

	/**
	@brief Gets projection matrix.

	@return The projection matrix.
	*/
	glm::mat4 getProjMatrix() { return projMatrix; }

	/**
	@brief Gets view matrix.

	@return The view matrix.
	*/
	glm::mat4 getViewMatrix() { return viewMatrix; }

	/**
	@brief Moves the Camera using the translation.
	@param translation The translation for the Camera.
	*/
	void moveCamera(glm::vec3 translation);

	/**
	@brief Moves the Camera along the X axis.
	@param translateX The X translation for the Camera.
	*/
	void moveCameraAlongX(float translateX);

	/**
	@brief Moves the Camera along the Y axis.
	@param translateY The Y translation for the Camera.
	*/
	void moveCameraAlongY(float translateY);

	/**
	@brief Moves the Camera along the Z axis.
	@param translateZ The Z translation for the Camera.
	*/
	void moveCameraAlongZ(float translateZ);

	/**
	@brief Rotates the Camera using the rotation.
	@param rotation The rotation for the Camera.
	*/
	void rotateCamera(glm::vec3 rotation);

	/**
	@brief Rotate the Camera along the X axis.
	@param rotationX The X rotation for the Camera.
	*/
	void rotateCameraAlongX(float rotationX);

	/**
	@brief Rotate the Camera along the Y axis.
	@param rotationY The Y rotation for the Camera.
	*/
	void rotateCameraAlongY(float rotationY);

	/**
	@brief Rotate the Camera along the Z axis.
	@param rotationZ The Z rotation for the Camera.
	*/
	void rotateCameraAlongZ(float rotationZ);

	/**
	 @brief Updates the view matrix to look at targetPos.	
	 @param targetPos Target position.
	void updateViewMat(glm::vec3 targetPos);*/

private:
	/** @brief The Orientation of the Camera*/
	glm::vec3 orientation;

	/** @brief The view matrix. */
	glm::mat4 viewMatrix;

	/** @brief The project matrix. */
	glm::mat4 projMatrix;
};