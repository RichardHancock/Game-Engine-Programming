#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "internal/dependencies/btBulletDynamicsCommon.h"
#include "misc/BulletDebugDrawer.h"
#include "misc/Utility.h"

/** @brief	The Static Physics Manager. */
class Physics
{
public:

	/** @brief	Initialise this Physics Manager. */
	static void init();

	/** @brief	Clean-ups this Physics Manager. */
	static void cleanup();

	/** @brief	Updates this Physics Manager. */
	static void update();

	/**
	 @brief	Gets the Bullet world.
	
	 @return	Null if it fails, else the world.
	 */
	static btDiscreteDynamicsWorld* getWorld();

	/**
	 @brief	Adds a rigid body.
	
	 @param [in,out]	rigidBody	If non-null, the rigid body.
	 */
	static void addRigidBody(btRigidBody* rigidBody);

	/**
	 @brief	Removes the rigid body described by rigidBody.
	
	 @param [in,out]	rigidBody	If non-null, the rigid body.
	 */
	static void removeRigidBody(btRigidBody* rigidBody);

	/**
	 @brief	Trigger explosion sphere.
	
	 @param	position	The position.
	 @param	radius  	The radius.
	 @param	duration	The duration.
	 */
	static void triggerExplosion(glm::vec3 position, float radius, float duration);

private:
	
	/** @brief	An explosion. */
	struct Explosion
	{
		glm::vec3 position;

		btCollisionShape* collisionShape;
		
		btMotionState* motionState;

		btRigidBody* rigidBody;

		Utility::SimpleTimer lifetime;
	};

	/** @brief	The explosions. */
	static std::vector<Explosion*> explosions;

	/** @brief	The Bullet world. */
	static btDiscreteDynamicsWorld* world;

	/** @brief	The constraint solver. */
	static btSequentialImpulseConstraintSolver* solver;

	/** @brief	The collision dispatcher. */
	static btCollisionDispatcher* dispatcher;

	/** @brief	The broad-phase interface. */
	static btBroadphaseInterface* broadphase;

	/** @brief	The collision configuration. */
	static btDefaultCollisionConfiguration* collisionConfiguration;

	/** @brief	The debug drawer. */
	static BulletDebugDrawer* debug;
};