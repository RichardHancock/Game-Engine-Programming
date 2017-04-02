#pragma once

#include <memory>

#include "internal/dependencies/btBulletDynamicsCommon.h"
#include "misc/BulletDebugDrawer.h"

class Physics
{
public:

	static void init();

	static void cleanup();

	static btDiscreteDynamicsWorld* getWorld();

	static void addRigidBody(btRigidBody* rigidBody);

	static void removeRigidBody(btRigidBody* rigidBody);

private:

	static btDiscreteDynamicsWorld* world;

	static btSequentialImpulseConstraintSolver* solver;

	static btCollisionDispatcher* dispatcher;

	static btBroadphaseInterface* broadphase;

	static btDefaultCollisionConfiguration* collisionConfiguration;

	static BulletDebugDrawer* debug;
};