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

	/** @brief	Destructor. */
	virtual ~Transform();

	/**
	 @brief	Gets the current world space postion.
	
	 @return	The postion.
	 */
	glm::vec3 getPostion();

	/**
	 @brief	Gets the current world space rotation.
	
	 @return	The rotation.
	 */
	glm::vec3 getRotation();

	/**
	 @brief	Gets the scale.
	 These are always local space.
	 @return	The scale.
	 */
	glm::vec3 getScale();

	/**
	 @brief	Sets the postion relative to parents.
	
	 @param	position	The position.
	 */
	void setPostion(glm::vec3 position);

	/**
	@brief	Sets the rotation relative to parents.

	@param	rotation	The rotation.
	*/
	void setRotation(glm::vec3 rotation);

	/**
	 @brief	Sets the scale.
	
	 @param	scale	The scale.
	 */
	void setScale(glm::vec3 scale);


	//Local Transform Components

	/**
	 @brief	Gets local space position.

	 @return	The local position.
	 */
	glm::vec3 getLocalPosition();

	/**
	 @brief	Gets local space rotation.
	
	 @return	The local rotation.
	 */
	glm::vec3 getLocalRotation();

	/**
	 @brief	Sets local position.
	
	 @param	position	The position.
	 */
	void setLocalPosition(glm::vec3 position);

	/**
	 @brief	Sets local rotation.
	
	 @param	rotation	The rotation.
	 */
	void setLocalRotation(glm::vec3 rotation);

	/**
	 @brief	Sets local scale.
	
	 @param	scale	The scale.
	 */
	void setLocalScale(glm::vec3 scale);


	/** @brief	Detatch all attached children. */
	void detatchChildren();

	/**
	 @brief	Gets the parent of this transform.
	
	 @return	The parent.
	 */
	std::weak_ptr<Transform> getParent();

	/**
	 @brief	Sets this transform's parent.
	
	 @param	transform	The parent transform.
	 */
	void setParent(std::weak_ptr<Transform> transform);

	/**
	 @brief	Gets the amount of transforms attached to this transform.
	
	 @return	The child count.
	 */
	int getChildCount();

	/**
	 @brief	Gets the child transform at specified index.
	
	 @param	index	Zero-based index of the transform.
	
	 @return	The child transform.
	 */
	std::weak_ptr<Transform> getChildTransform(int index);

	/**
	 @brief	Gets the root of this transform hierachy.
	
	 @return	The root transform.
	 */
	std::shared_ptr<Transform> getRootTransform();

	/**
	 @brief	Searches for the specified transform node.
	
	 @param	query	The query.
	
	 @return	The found transform node.
	 */
	std::weak_ptr<Transform> findTransformNode(std::string query);

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
	 @brief	Rotate around position.
	
	 @param	centerPoint	The center point.
	 @param	rotateAxis 	The rotate axis. EX Vec3(1.0f,0.0f,0.0f) for X axis
	 @param	amount	   	The amount to rotate by.
	 */
	//void rotateAroundPos(glm::vec3 centerPoint, glm::vec3 rotateAxis, float amount);

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
	 @param	rotation	The rotation.
	 @param	scale   	The scale.
	
	 @return	A glm::mat4.
	 */
	static glm::mat4 buildTransformMat(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

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
	glm::vec3 localRotation;
	
	btQuaternion quat;

	/** @brief	The local scale. */
	glm::vec3 localScale;
	/** @brief	The parent transform. */
	std::weak_ptr<Transform> parent;
	/** @brief	The transforms this transform is a parent to. */
	std::vector<std::shared_ptr<Transform>> children;

	//A Bad hack to get lookAts working, otherwise would require full redesign of Transform class
	glm::mat4 lookAtMat;
	bool useLookAt;
};
