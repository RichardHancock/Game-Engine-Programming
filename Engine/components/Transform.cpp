#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../misc/Utility.h"
#include "RigidBody.h"

Transform::Transform()
{
	localScale = glm::vec3(1);
	quat = quat.getIdentity();

	useLookAt = false;
}

Transform::~Transform()
{
}

glm::vec3 Transform::getPosition()
{
	///@todo Implement fully
	return localPosition;
}

btQuaternion Transform::getRotation()
{
	return quat;
}

glm::vec3 Transform::getEulerRotation()
{
	return Utility::bulletVec3ToGLM(quat.getAxis() * quat.getAngle());
}

glm::vec3 Transform::getScale()
{
	return localScale;
}

void Transform::setPosition(glm::vec3 position, bool updatePhysics)
{
	localPosition = position;
	
	if (updatePhysics)
	{
		auto rigidbodyRef = getGameObject().lock()->getComponent<RigidBody>("RigidBody");

		if (rigidbodyRef.expired())
			return;

		rigidbodyRef.lock()->setPosition(localPosition);
	}
}

void Transform::setEulerRotation(glm::vec3 rotation)
{
	quat = quat.getIdentity();
	quat.setEulerZYX(rotation.z, rotation.y, rotation.x);

	auto rigidbodyRef = getGameObject().lock()->getComponent<RigidBody>("RigidBody");

	if (rigidbodyRef.expired())
		return;

	rigidbodyRef.lock()->setRotation(quat);
}

void Transform::setRotation(btQuaternion & rotation)
{
	quat = rotation;
}

void Transform::setScale(glm::vec3 scale)
{
	localScale = scale;
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
	btQuaternion rotationQuat;
	rotationQuat.setEulerZYX(rotation.z, rotation.y, rotation.x);

	quat = quat * rotationQuat;

	auto rigidbodyRef = getGameObject().lock()->getComponent<RigidBody>("RigidBody");

	if (rigidbodyRef.expired())
		return;

	rigidbodyRef.lock()->setRotation(quat);
}

void Transform::lookAt(glm::vec3 worldPosition)
{
	lookAtMat = glm::lookAt(localPosition, worldPosition, getUpVector());
	useLookAt = true;
}

/*
void Transform::rotateAroundPos(glm::vec3 centerPoint, glm::vec3 rotateAxis, float amount)
{
	glm::mat4 pos = buildTransformMat(centerPoint, glm::vec3(0), glm::vec3(1));

	glm::mat4 rot = buildTransformMat(glm::vec3(0), glm::vec3(rotateAxis * amount), glm::vec3(1));

	glm::mat4 current = glm::mat4(1);

	
	current = current * pos;
	current = current * rot;
	current = current * glm::inverse(pos);

	localPosition = vec4ToVec3(current * glm::vec4(localPosition, 1));
}*/

glm::vec3 Transform::getForwardVector()
{
	glm::mat4 mat = buildTransformMat(localPosition, quat, localScale);
	glm::vec4 vec(0, 0, 1, 0);

	return vec4ToVec3(mat * vec);
}

glm::vec3 Transform::getUpVector()
{
	glm::mat4 mat = buildTransformMat(localPosition, quat, localScale);
	glm::vec4 vec(0, 1, 0, 0);

	return vec4ToVec3(mat * vec);
}

glm::vec3 Transform::getRightVector()
{
	glm::mat4 mat = buildTransformMat(localPosition, quat, localScale);
	glm::vec4 vec(1, 0, 0, 0);

	return vec4ToVec3(mat * vec);
}

glm::mat4 Transform::getTransformMat()
{
	if (useLookAt)
	{
		return lookAtMat;
	}
	else
	{
		return buildTransformMat(getPosition(), getRotation(), getScale());
	}
}

glm::mat4 Transform::buildTransformMat(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 mat = glm::mat4(1);

	mat = glm::translate(mat, position);
	
	mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));
	mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));

	//This may need to done first
	mat = glm::scale(mat, scale);

	return mat;
}

glm::mat4 Transform::buildTransformMat(glm::vec3 position, btQuaternion& rotation, glm::vec3 scale)
{
	glm::mat4 mat;

	//mat = glm::translate(mat, position);

	//glm::vec3 eulerRotation = Utility::bulletVec3ToGLM(rotation.getAxis() * rotation.getAngle());
	
	//glm::quat tempQuat(rotation.x(), rotation.y(), rotation.z(), rotation.w());

	//mat = glm::rotate(mat, eulerRotation.z, glm::vec3(0, 0, 1));
	//mat = glm::rotate(mat, eulerRotation.y, glm::vec3(0, 1, 0));
	//mat = glm::rotate(mat, eulerRotation.x, glm::vec3(1, 0, 0));

	//mat = mat * glm::mat4_cast(tempQuat);

	//I HATE QUATERNIONS, LOOK WHAT THEY MADE ME DO ;(
	btTransform temp;
	temp = temp.getIdentity();

	temp.setOrigin(Utility::glmToBulletVec3(position));
	temp.setRotation(rotation);

	float matrixRaw[16];

	temp.getOpenGLMatrix(matrixRaw);

	mat = glm::make_mat4(matrixRaw);
	//mat = glm::translate(mat, position);

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
	
}

void Transform::onDestroy()
{
}
