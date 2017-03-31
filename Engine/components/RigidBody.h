#pragma once

#include "Component.h"

#include <glm/glm.hpp>

#include "../internal/dependencies/btBulletDynamicsCommon.h"
#include "CollisionShape.h"

class RigidBody : public Component
{
public:
	RigidBody();

	void setPosition(glm::vec3 newPos);

	void setRotation(glm::vec3 newRotation);
	
	virtual void onAwake();

	void applyForce(glm::vec3 newForce);

	void init(float weight);

private:
	btRigidBody* rigidBody;

	btMotionState* motionState;

	//btVector3 position;
};