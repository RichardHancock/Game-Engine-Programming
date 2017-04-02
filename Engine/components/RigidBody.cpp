#include "RigidBody.h"

#include "../Physics.h"
#include "Transform.h"
#include "../misc/Utility.h"
#include "../misc/MotionState.h"

RigidBody::RigidBody()
	: Component()
{
	rigidBody = nullptr;
	motionState = nullptr;

	//position = btVector3(0, 0, 0);
}

RigidBody::~RigidBody()
{
	Physics::removeRigidBody(rigidBody);

	delete rigidBody;
	delete motionState;
}


void RigidBody::setPosition(glm::vec3 newPos)
{
	if (rigidBody != nullptr)
	{
		btTransform current = rigidBody->getWorldTransform();
		current.setOrigin(Utility::glmToBulletVec3(newPos));

		rigidBody->setActivationState(ACTIVE_TAG);
		rigidBody->setWorldTransform(current);
	}
}

void RigidBody::setRotation(btQuaternion& newRotation)
{
	if (rigidBody != nullptr)
	{
		btTransform current = rigidBody->getWorldTransform();

		current.setRotation(newRotation);

		rigidBody->setActivationState(ACTIVE_TAG);
		rigidBody->setWorldTransform(current);
	}
}

void RigidBody::setDamping(float linearDamping, float angularDamping)
{
	rigidBody->setDamping(linearDamping, angularDamping);
}

void RigidBody::onAwake()
{
	
}

void RigidBody::applyForce(glm::vec3 newForce)
{
	rigidBody->setActivationState(ACTIVE_TAG);
	rigidBody->applyCentralImpulse(Utility::glmToBulletVec3(newForce));
}

void RigidBody::init(float weight, glm::vec3 inertia)
{
	auto transformRef = getGameObject().lock()->getComponent<Transform>("Transform");
	assert(!transformRef.expired());

	auto transform = transformRef.lock();
	//setPosition(transform->getPostion());
	
	motionState = new MotionState(transformRef);

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
	collisionShape->setLocalScaling(Utility::glmToBulletVec3(transform->getScale()));

	btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(
		weight, motionState, collisionShape, Utility::glmToBulletVec3(inertia)
	);

	rigidBody = new btRigidBody(rigidBodyInfo);
	
	//rigidBody->rot
	//rigidBody->setRestitution(1.0f);
	

	Physics::addRigidBody(rigidBody);
}
