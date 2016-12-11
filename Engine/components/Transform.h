#pragma once

#include <memory>
#include <vector>
#include <glm/matrix.hpp>

#include "Component.h"
#include "../misc/Vec3.h"


class Transform : public Component, public std::enable_shared_from_this<Transform>
{
public:

	friend class GameObject;

	virtual ~Transform();

	glm::vec3 getPostion();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	void setPostion(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);


	//Local Transform Components

	/**
	 @brief	Gets local position.

	 @return	The local position.
	 */
	glm::vec3 getLocalPosition();
	glm::vec3 getLocalRotation();
	void setLocalPosition(glm::vec3 position);
	void setLocalRotation(glm::vec3 rotation);
	void setLocalScale(glm::vec3 scale);


	void detatchChildren();
	std::weak_ptr<Transform> getParent();
	void setParent(std::weak_ptr<Transform> transform);
	int getChildCount();
	std::weak_ptr<Transform> getChildTransform(int index);
	std::shared_ptr<Transform> getRootTransform();
	std::weak_ptr<Transform> findTransformNode(std::string query);


	void translate(glm::vec3 translation);
	void rotate(glm::vec3 rotation);
	void lookAt(glm::vec3 worldPosition);
	void rotateAroundPos(glm::vec3 centerPoint, glm::vec3 rotateAxis, float amount);


	glm::vec3 getForwardVector();
	glm::vec3 getRightVector();


	glm::mat4 getTransformMat();


	static glm::mat4 buildTransformMat(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	static glm::vec3 vec4ToVec3(const glm::vec4& vector);

private:

	glm::vec3 localPosition;
	glm::vec3 localRotation;
	glm::vec3 localScale;
	std::weak_ptr<Transform> parent;
	std::vector<std::shared_ptr<Transform>> children;

	virtual void onAwake();
	virtual void onDestroy();


};
