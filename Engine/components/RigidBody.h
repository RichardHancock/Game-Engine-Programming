#pragma once

#include "Component.h"

//This is done because bullet is full of warnings and Visual Studio pollutes my warnings with them. This only disables warning in bullet, not my code.
#pragma warning(push, 0) 
#include <btBulletDynamicsCommon.h>
#pragma warning(pop)

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