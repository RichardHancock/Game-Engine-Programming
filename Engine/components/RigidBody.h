#pragma once

#include "Component.h"

#include <glm/glm.hpp>

#include "../internal/dependencies/btBulletDynamicsCommon.h"
#include "CollisionShape.h"

class RigidBody : public Component
{
public:
	RigidBody();

	~RigidBody();

	void setPosition(glm::vec3 newPos);

	void setRotation(btQuaternion& newRotation);
	
	void setDamping(float linearDamping, float angularDamping);

	void setRestitution(float value);

	virtual void onAwake();

	void applyForce(glm::vec3 newForce);

	void init(float weight, glm::vec3 inertia);

private:
	btRigidBody* rigidBody;

	btMotionState* motionState;

	//btVector3 position;
};