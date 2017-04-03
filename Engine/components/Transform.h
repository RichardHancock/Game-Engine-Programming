#pragma once

#include <memory>
#include <vector>
#include <glm/matrix.hpp>

#include "Component.h"
#include "../misc/Vec3.h"
#include "../internal/dependencies/btBulletDynamicsCommon.h"

//Ref: Karsten (Mutiny) with changes to make it more suitable for my engine.

/** @brief	Component that contains all the transformation data for a game object */
class Transform : public Component, public std::enable_shared_from_this<Transform>
{
public:

	friend class GameObject;

	/** @brief	Default constructor. */
	Transform();

	/** @brief	Destructor. */
	virtual ~Transform();

	/**
	 @brief	Gets the current world space position.
	
	 @return	The position.
	 */
	glm::vec3 getPosition();

	/**
	 @brief	Gets the current world space rotation.
	
	 @return	The rotation.
	 */
	btQuaternion getRotation();

	/**
	@brief	Gets the current world space rotation in Euler Angles.

	@return	The rotation.
	*/
	glm::vec3 getEulerRotation();

	/**
	 @brief	Gets the scale.
	 These are always local space.
	 @return	The scale.
	 */
	glm::vec3 getScale();

	/**
	 @brief	Sets the position relative to parents.
	
	 @param	position	 	The position.
	 @param	updatePhysics	(Optional) True to update physics (Usually Leave this on).
	 */
	void setPosition(glm::vec3 position, bool updatePhysics = true);

	/**
	@brief	Sets the rotation using Euler Angles.

	@param	rotation	The rotation.
	*/
	void setEulerRotation(glm::vec3 rotation);

	/**
	@brief	Sets the rotation.

	@param	rotation	The rotation.
	*/
	void setRotation(btQuaternion& rotation);

	/**
	 @brief	Sets the scale.
	
	 @param	scale	The scale.
	 */
	void setScale(glm::vec3 scale);


	/**
	 @brief	Translates the position by the given translation.
	
	 @param	translation	The translation.
	 */
	void translate(glm::vec3 translation);

	/**
	 @brief	Rotates the transform by the given rotation.
	
	 @param	rotation	The rotation.
	 */
	void rotate(glm::vec3 rotation);

	/**
	 @brief	Generated a look at matrix pointing at the passed in position in world space.
	
	 @param	worldPosition	The world position.
	 */
	void lookAt(glm::vec3 worldPosition);

	/**
	 @brief	Gets forward vector.
	
	 @return	The forward vector.
	 */
	glm::vec3 getForwardVector();

	/**
	@brief	Gets up vector.

	@return	The up vector.
	*/
	glm::vec3 getUpVector();
	
	/**
	 @brief	Gets right vector.
	
	 @return	The right vector.
	 */
	glm::vec3 getRightVector();

	/**
	 @brief	Gets the resulting transformation matrix.
	
	 @return	The transform matrix.
	 */
	glm::mat4 getTransformMat();

	/**
	 @brief	Builds a transform matrix from passed in elements.
	
	 @param	position	The position.
	 @param	rotation	The rotation as Euler angles.
	 @param	scale   	The scale.
	
	 @return	A glm::mat4.
	 */
	static glm::mat4 buildTransformMat(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	/**
	@brief	Builds a transform matrix from passed in elements.

	@param	position	The position.
	@param	rotation	The rotation as a Quaternion.
	@param	scale   	The scale.

	@return	A glm::mat4.
	*/
	static glm::mat4 buildTransformMat(glm::vec3 position, btQuaternion& rotation, glm::vec3 scale);


	/**
	 @brief	Helper function to convert a GLM Vector 4 to vector 3.
	
	 @param	vector	The vector.
	
	 @return	A glm::vec3.
	 */
	static glm::vec3 vec4ToVec3(const glm::vec4& vector);
	
	/** @brief	Executes the awake action. */
	virtual void onAwake();
	/** @brief	Executes the destroy action. */
	virtual void onDestroy();
private:

	/** @brief	The local position. */
	glm::vec3 localPosition;
	
	/** @brief	The local rotation. */
	//glm::vec3 localRotation;
	
	/** @brief	The quaternion rotation. */
	btQuaternion quat;

	/** @brief	The local scale. */
	glm::vec3 localScale;

	//A Bad hack to get lookAts working, otherwise would require full redesign of Transform class
	/** @brief	The look at matrix. */
	glm::mat4 lookAtMat;
	/** @brief	True to use look at else generate matrix from transform components. */
	bool useLookAt;
};
