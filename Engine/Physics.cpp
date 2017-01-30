#include "Physics.h"

std::shared_ptr<btDiscreteDynamicsWorld> Physics::world;
std::shared_ptr<btSequentialImpulseConstraintSolver> Physics::solver;
std::shared_ptr<btCollisionDispatcher> Physics::dispatcher;
std::shared_ptr<btBroadphaseInterface> Physics::broadphase;
std::shared_ptr<btDefaultCollisionConfiguration> Physics::collisionConfiguration;

void Physics::init()
{
	broadphase = std::make_shared<btDbvtBroadphase>();

	collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();

	dispatcher = std::make_shared<btCollisionDispatcher>(collisionConfiguration.get());

	solver = std::make_shared<btSequentialImpulseConstraintSolver>();

	world = std::make_shared<btDiscreteDynamicsWorld>(
		dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get());

	world->setGravity(btVector3(0, -1, 0));
}
