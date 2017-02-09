#include "RigidBody.h"

#include "../Physics.h"
#include "Transform.h"

RigidBody::RigidBody()
	: Component()
{
	rigidBody = nullptr;
	motionState = nullptr;

	position = btVector3(0, 0, 0);
}

void RigidBody::setPosition(glm::vec3 newPos)
{
	position = btVector3(newPos.x + 40.0f, newPos.y, newPos.z + 5.0f);
	
	if (rigidBody != nullptr)
	{
		rigidBody->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position)));
	}
	
}

void RigidBody::onAwake()
{
	
}

void RigidBody::init()
{
	auto transformRef = getGameObject().lock()->getComponent<Transform>("Transform");
	assert(!transformRef.expired());

	auto transform = transformRef.lock();
	setPosition(transform->getPostion());
	
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), position));

	auto shapeRef = getGameObject().lock()->getComponent<CollisionShape>("CollisionShape");

	if (shapeRef.expired())
	{
		Log::logE("Rigid Body component can not be created before a collision shape is attached to the game object.");
		return;
	}

	auto collisionShapeRef = shapeRef.lock()->getShape();
	/*if (collisionShapeRef.expired())
	{
	Log::logE("Rigid Body component can not be created as the Collision shape component does not have a valid Shape.");
	return;
	}*/

	auto collisionShape = collisionShapeRef;

	btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
		0, motionState, collisionShape, btVector3(0, 0, 0)
	);

	rigidBody = new btRigidBody(rigidBodyInfo);

	Physics::addRigidBody(rigidBody);
}