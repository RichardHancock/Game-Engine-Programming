#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>

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