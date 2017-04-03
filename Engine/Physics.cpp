#include "Physics.h"

#include <string>

#include "misc/Log.h"
#include "misc/Utility.h"
#include "misc/GameVariables.h"
#include "GameObject.h"

btDiscreteDynamicsWorld* Physics::world;
btSequentialImpulseConstraintSolver* Physics::solver;
btCollisionDispatcher* Physics::dispatcher;
btBroadphaseInterface* Physics::broadphase;
btDefaultCollisionConfiguration* Physics::collisionConfiguration;
BulletDebugDrawer* Physics::debug;
std::vector<Physics::Explosion*> Physics::explosions;

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

	world->setGravity(btVector3(0, 0, 0));

	debug = new BulletDebugDrawer();
	debug->setDebugMode(BulletDebugDrawer::DBG_DrawWireframe + BulletDebugDrawer::DBG_DrawContactPoints); //+ BulletDebugDrawer::DBG_DrawNormals);
	world->setDebugDrawer((btIDebugDraw*)debug);
}

void Physics::cleanup()
{
	while (explosions.size() > 0)
	{
		Explosion* explosion = explosions[0];

		
		world->removeRigidBody(explosion->rigidBody);

		delete explosion->rigidBody;
		delete explosion->motionState;
		delete explosion->collisionShape;

		delete explosion;
		explosions.erase(explosions.begin());
	}

	delete world;
	world = nullptr;
	delete debug;
	debug = nullptr;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}

void Physics::update()
{
	//Update Collisions
	for (unsigned int explosionID = 0; explosionID < explosions.size(); explosionID++)
	{
		Explosion* explosion = explosions[explosionID];
		explosion->lifetime.update();

		if (explosion->lifetime.hasTimerFinished())
		{
			world->removeRigidBody(explosion->rigidBody);

			delete explosion->rigidBody;
			delete explosion->motionState;
			delete explosion->collisionShape;

			delete explosion;
			explosions.erase(explosions.begin() + explosionID);

			explosionID--;
		}
	}


	//Send Collision Calls
	int numManifolds = dispatcher->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());
		
		//if (obA->checkCollideWith(obB))
		if (contactManifold->getNumContacts() > 0)
		{
			std::string* obAName = (std::string*)obA->getUserPointer();
			std::string* obBName = (std::string*)obB->getUserPointer();
			btManifoldPoint point = contactManifold->getContactPoint(0);

			if (*obAName == "Explosion" || *obBName == "Explosion")
				return;

			GameVariables::data->gameObjs[*obAName]->onCollision(*obAName, *obBName, Utility::bulletVec3ToGLM(point.getPositionWorldOnA()));
			GameVariables::data->gameObjs[*obBName]->onCollision(*obBName, *obAName, Utility::bulletVec3ToGLM(point.getPositionWorldOnB()));
			Log::logI("Collision: A: " + *obAName + ", B: " + *obBName);
		}
	}
}

btDiscreteDynamicsWorld* Physics::getWorld()
{
	return world;
}

void Physics::addRigidBody(btRigidBody* rigidBody)
{
	if (world != nullptr)
		world->addRigidBody(rigidBody);
}

void Physics::removeRigidBody(btRigidBody * rigidBody)
{
	if (world != nullptr)
		world->removeRigidBody(rigidBody);
}

void Physics::triggerExplosion(glm::vec3 position, float radius, float duration)
{
	btCollisionShape* collisionShape = new btSphereShape(radius);
	collisionShape->setMargin(2.0f);

	btMotionState* motionState = new btDefaultMotionState(
		btTransform(btQuaternion().getIdentity(), Utility::glmToBulletVec3(position)));

	btRigidBody* rb = new btRigidBody(0.0f, motionState, collisionShape);

	Explosion* e = new Explosion();
	e->position = position;
	e->collisionShape = collisionShape;
	e->motionState = motionState;
	e->rigidBody = rb;
	e->lifetime.restart(duration);

	explosions.push_back(e);
	
	rb->setUserPointer(new std::string("Explosion"));
	rb->setRestitution(5.0f);
	world->addRigidBody(rb);
}
