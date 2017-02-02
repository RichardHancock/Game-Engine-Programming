#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../misc/Utility.h"
#include "RigidBody.h"

Transform::~Transform()
{
}

glm::vec3 Transform::getPostion()
{
	///@todo Implement fully
	return localPosition;
}

glm::vec3 Transform::getRotation()
{
	return localRotation;
}

glm::vec3 Transform::getScale()
{
	return localScale;
}

void Transform::setPostion(glm::vec3 position)
{
	if (getParent().lock().get() != nullptr)
	{
		glm::mat4 mat = glm::mat4(1);
		std::weak_ptr<Transform> curParent = getParent();

		while (curParent.lock().get() != nullptr)
		{
			std::shared_ptr<Transform> node = curParent.lock();
			mat = (buildTransformMat(node->localPosition, node->localRotation, glm::vec3(1)) * mat);

			curParent = node->getParent();
		}

		localPosition = vec4ToVec3((glm::inverse(mat) * glm::vec4(position, 1)));
	}
	else
	{
		localPosition = position;
	}

	auto rigidbodyRef = getGameObject().lock()->getComponent<RigidBody>("RigidBody");

	if (rigidbodyRef.expired())
		return;

	rigidbodyRef.lock()->setPosition(localPosition);
}

void Transform::setRotation(glm::vec3 rotation)
{
	if (getParent().lock().get() != nullptr)
	{
		localRotation = rotation - getParent().lock()->getRotation();
	}
	else
	{
		localRotation = rotation;
	}
}

void Transform::setScale(glm::vec3 scale)
{
	if (getParent().lock().get() != nullptr)
	{
		glm::mat4 mat = glm::mat4(1);
		std::weak_ptr<Transform> curParent = getParent();

		while (curParent.lock().get() != nullptr)
		{
			std::shared_ptr<Transform> node = curParent.lock();
			mat = (buildTransformMat(node->localPosition, node->localRotation, node->localScale) * mat);

			curParent = node->getParent();
		}

		localScale = vec4ToVec3(glm::inverse(mat) * glm::vec4(scale, 1));
	}
	else
	{
		localScale = scale;
	}
}

glm::vec3 Transform::getLocalPosition()
{
	return localPosition;
}

glm::vec3 Transform::getLocalRotation()
{
	return localRotation;
}

void Transform::setLocalPosition(glm::vec3 position)
{
	localPosition = position;

	auto rigidbodyRef = getGameObject().lock()->getComponent<RigidBody>("RigidBody");

	if (rigidbodyRef.expired())
		return;

	rigidbodyRef.lock()->setPosition(localPosition);
}

void Transform::setLocalRotation(glm::vec3 rotation)
{
	localRotation = rotation;
}

void Transform::setLocalScale(glm::vec3 scale)
{
	localScale = scale;
}

void Transform::detatchChildren()
{
	for (auto child : children)
	{
		child->setParent(std::weak_ptr<Transform>());
		///TODO: Check why he calls delete on entire object here.
	}

	children.clear();
}

std::weak_ptr<Transform> Transform::getParent()
{
	return parent;
}

void Transform::setParent(std::weak_ptr<Transform> transform)
{
	std::shared_ptr<Transform> parPtr = getParent().lock();

	if (parPtr.get() != nullptr)
	{
		for (unsigned int i = 0; i < parPtr->children.size(); i++)
		{
			if (parPtr->children[i].get() == this)
			{
				parPtr->children.erase(parPtr->children.begin() + i);
				break;
			}
		}
	}

	std::shared_ptr<Transform> trPtr = transform.lock();
	if (trPtr.get() != nullptr)
	{
		trPtr->children.push_back(std::shared_ptr<Transform>(this));
	}

	setLocalPosition(getPostion());
	setLocalRotation(getRotation());
	parent = transform;
	setPostion(getLocalPosition());
	setRotation(getLocalRotation());
}

int Transform::getChildCount()
{
	return children.size();
}

std::weak_ptr<Transform> Transform::getChildTransform(int index)
{
	return children.at(index);
}

std::shared_ptr<Transform> Transform::getRootTransform()
{
	std::shared_ptr<Transform> root(this);
	std::weak_ptr<Transform> trPtr = parent;

	while (trPtr.lock().get() != nullptr)
	{
		root = trPtr.lock();
		trPtr = root->getParent();
	}

	return root;
}

std::weak_ptr<Transform> Transform::findTransformNode(std::string query)
{
	for (auto child : children)
	{
		if (child->getGameObject().lock()->getName() == query)
		{
			return child;
		}
	}

	return std::weak_ptr<Transform>();
}

void Transform::translate(glm::vec3 translation)
{
	localPosition += translation;

	auto rigidbodyRef = getGameObject().lock()->getComponent<RigidBody>("RigidBody");

	if (rigidbodyRef.expired())
		return;

	rigidbodyRef.lock()->setPosition(localPosition);
}

void Transform::rotate(glm::vec3 rotation)
{
	localRotation += rotation;
}

void Transform::lookAt(glm::vec3 worldPosition)
{
	//TODO: REWRITE
	glm::vec2 diff = glm::vec2(localPosition.x, localPosition.z) - glm::vec2(worldPosition.x, worldPosition.z);

	float angle = atan2(diff.x, diff.x) * 180 / Utility::PI;
	localRotation.y = angle - 180.0f;
}

void Transform::rotateAroundPos(glm::vec3 centerPoint, glm::vec3 rotateAxis, float amount)
{
	glm::mat4 pos = buildTransformMat(centerPoint, glm::vec3(0), glm::vec3(1));

	glm::mat4 rot = buildTransformMat(glm::vec3(0), glm::vec3(rotateAxis * amount), glm::vec3(1));

	glm::mat4 current = glm::mat4(1);

	
	current = current * pos;
	current = current * rot;
	current = current * glm::inverse(pos);

	localPosition = vec4ToVec3(current * glm::vec4(localPosition, 1));
}

glm::vec3 Transform::getForwardVector()
{
	glm::mat4 mat = buildTransformMat(localPosition, localRotation, localScale);
	glm::vec4 vec(0, 0, 1, 0);

	return vec4ToVec3(mat * vec);
}

glm::vec3 Transform::getUpVector()
{
	glm::mat4 mat = buildTransformMat(localPosition, localRotation, localScale);
	glm::vec4 vec(0, 1, 0, 0);

	return vec4ToVec3(mat * vec);
}

glm::vec3 Transform::getRightVector()
{
	glm::mat4 mat = buildTransformMat(localPosition, localRotation, localScale);
	glm::vec4 vec(1, 0, 0, 0);

	return vec4ToVec3(mat * vec);
}

glm::mat4 Transform::getTransformMat()
{
	return buildTransformMat(getPostion(), getRotation(), getScale());
}

glm::mat4 Transform::buildTransformMat(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 mat = glm::mat4(1);

	mat = glm::translate(mat, position);
	
	//XYZ Rotation Order
	mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));
	mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));

	//This may need to done first
	mat = glm::scale(mat, scale);

	return mat;
}

glm::vec3 Transform::vec4ToVec3(const glm::vec4& vector)
{
	return glm::vec3(vector.x, vector.y, vector.z);
}

void Transform::onAwake()
{
	///TODO does this require parent to be intialised.
	localScale = glm::vec3(1);
}

void Transform::onDestroy()
{
	detatchChildren();
	setParent(std::weak_ptr<Transform>());
}
