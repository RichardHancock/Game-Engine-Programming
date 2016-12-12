#pragma once

#include "Component.h"

#include <glm/glm.hpp>

/** @brief	A camera component that stores the projection matrix. */
class Camera : public Component
{
public:
	
	/** @brief	Destructor. */
	virtual ~Camera();

	/**
	 @brief	Gets projection matrix.
	
	 @return	The projection matrix.
	 */
	glm::mat4 getProjMat();

	/**
	 @brief	Sets projection matrix.
	
	 @param [in,out]	newProjMatrix	The new projection matrix.
	 */
	void setProjMat(glm::mat4& newProjMatrix);

	/**
	 @brief	Gets transform matrix from the transform component. This should be used as the view matrix
	
	 @return	The transform matrix.
	 */
	glm::mat4 getTransformMat();

	/** @brief	Executes the awake action. */
	virtual void onAwake();

private:

	/** @brief	The projection matrix. */
	glm::mat4 projMatrix;
};