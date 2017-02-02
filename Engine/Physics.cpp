#include "Physics.h"

btDiscreteDynamicsWorld* Physics::world;
btSequentialImpulseConstraintSolver* Physics::solver;
btCollisionDispatcher* Physics::dispatcher;
btBroadphaseInterface* Physics::broadphase;
btDefaultCollisionConfiguration* Physics::collisionConfiguration;
BulletDebugDrawer* Physics::debug;


void Physics::init()
{
	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();

	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(
		dispatcher, broadphase, solver, collisionConfiguration
	);

	world->setGravity(btVector3(0, -1, 0));

	debug = new BulletDebugDrawer();
	debug->setDebugMode(BulletDebugDrawer::DBG_DrawWireframe);
	world->setDebugDrawer((btIDebugDraw*)debug);
}

btDiscreteDynamicsWorld* Physics::getWorld()
{
	return world;
}

void Physics::addRigidBody(btRigidBody* rigidBody)
{
	world->addRigidBody(rigidBody);
}
