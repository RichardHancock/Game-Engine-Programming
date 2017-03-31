#include "MotionState.h"

#include "Utility.h"

MotionState::MotionState(std::weak_ptr<Transform> linkedTransformRef)
	: btMotionState()
{
	transformRef = linkedTransformRef;

	if (linkedTransformRef.expired())
		Log::logW("Transform Ref passed to motion state has expired, this will cause problems if used");
}


void MotionState::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans.setIdentity();

	auto transform = transformRef.lock();
	glm::vec3 position = transform->getPostion();
	worldTrans.setOrigin(Utility::glmToBulletVec3(position));

	glm::vec3 rotation = transform->getRotation();

	btQuaternion quat;
	quat.setEulerZYX(rotation.z, rotation.y, rotation.x);

	worldTrans.setRotation(quat);
}

void MotionState::setWorldTransform(const btTransform& worldTrans)
{
	auto transform = transformRef.lock();

	transform->setPostion(Utility::bulletVec3ToGLM(worldTrans.getOrigin()));

	btQuaternion quat = worldTrans.getRotation();


	glm::vec3 rotation;
	//worldTrans.getBasis().getEulerYPR(rotation.x, rotation.y, rotation.z);
	worldTrans.getBasis().getEulerZYX(rotation.z, rotation.y, rotation.x);

	//rotation.x = -rotation.x;
	//rotation.y = -rotation.y;

	//transform->setRotation(Utility::bulletVec3ToGLM(quat.getAxis() * quat.getAngleShortestPath()));
	transform->setRotation(rotation);
}
