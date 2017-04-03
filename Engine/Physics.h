#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "internal/dependencies/btBulletDynamicsCommon.h"
#include "misc/BulletDebugDrawer.h"
#include "misc/Utility.h"

class Physics
{
public:

	static void init();

	static void cleanup();

	static void update();

	static btDiscreteDynamicsWorld* getWorld();

	static void addRigidBody(btRigidBody* rigidBody);

	static void removeRigidBody(btRigidBody* rigidBody);
	
	static void triggerExplosion(glm::vec3 position, float radius, float duration);

private:
	
	struct Explosion
	{
		glm::vec3 position;

		btCollisionShape* collisionShape;
		
		btMotionState* motionState;

		btRigidBody* rigidBody;

		Utility::SimpleTimer lifetime;
	};

	static std::vector<Explosion*> explosions;

	static btDiscreteDynamicsWorld* world;

	static btSequentialImpulseConstraintSolver* solver;

	static btCollisionDispatcher* dispatcher;

	static btBroadphaseInterface* broadphase;

	static btDefaultCollisionConfiguration* collisionConfiguration;

	static BulletDebugDrawer* debug;
};