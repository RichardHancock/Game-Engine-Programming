#include "Physics.h"

#include <string>

#include "misc/Log.h"
#include "misc/Utility.h"

btDiscreteDynamicsWorld* Physics::world;
btSequentialImpulseConstraintSolver* Physics::solver;
btCollisionDispatcher* Physics::dispatcher;
btBroadphaseInterface* Physics::broadphase;
btDefaultCollisionConfiguration* Physics::collisionConfiguration;
BulletDebugDrawer* Physics::debug;

void Physics::init()
{
	std::string version = Utility::intToString(btGetVersion());
	std::string versionMajor = version.substr(0, 1);
	std::string versionMinor = version.substr(1, std::string::npos);

	Log::logI("Physics SubSystem Initialized using Bullet Physics Library V" + versionMajor + "." + versionMinor);

	broadphase = new btDbvtBroadphase();

	collisionConfiguration = new btDefaultCollisionConfiguration();

	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(
		dispatcher, broadphase, solver, collisionConfiguration
	);

	world->setGravity(btVector3(0, -1, 0));

	debug = new BulletDebugDrawer();
	debug->setDebugMode(BulletDebugDrawer::DBG_DrawWireframe + BulletDebugDrawer::DBG_DrawContactPoints + BulletDebugDrawer::DBG_DrawNormals);
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
