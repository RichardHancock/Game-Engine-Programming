#pragma once

#include "Component.h"

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "CollisionShape.h"

class RigidBody : public Component
{
public:
	RigidBody();

	void setPosition(glm::vec3 newPos);
	
	virtual void onAwake();

	void init();

private:
	btRigidBody* rigidBody;

	btDefaultMotionState* motionState;

	btVector3 position;
};