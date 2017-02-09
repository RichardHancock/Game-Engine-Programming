#pragma once

#include <memory>

//This is done because bullet is full of warnings and Visual Studio pollutes my warnings with them. This only disables warning in bullet, not my code.
#pragma warning(push, 0) 
#include <btBulletDynamicsCommon.h>
#pragma warning(pop)

#include "misc/BulletDebugDrawer.h"

class Physics
{
public:

	static void init();

	static btDiscreteDynamicsWorld* getWorld();

	static void addRigidBody(btRigidBody* rigidBody);

private:

	static btDiscreteDynamicsWorld* world;

	static btSequentialImpulseConstraintSolver* solver;

	static btCollisionDispatcher* dispatcher;

	static btBroadphaseInterface* broadphase;

	static btDefaultCollisionConfiguration* collisionConfiguration;

	static BulletDebugDrawer* debug;
};