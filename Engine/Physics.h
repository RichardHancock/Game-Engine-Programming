#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>

class Physics
{
public:

	static void init();

private:

	static std::shared_ptr<btDiscreteDynamicsWorld> world;

	static std::shared_ptr<btSequentialImpulseConstraintSolver> solver;

	static std::shared_ptr<btCollisionDispatcher> dispatcher;

	static std::shared_ptr<btBroadphaseInterface> broadphase;

	static std::shared_ptr<btDefaultCollisionConfiguration> collisionConfiguration;

	
};