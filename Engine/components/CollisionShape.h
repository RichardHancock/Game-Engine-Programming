#pragma once

#include <memory>

#include "../internal/dependencies/btBulletDynamicsCommon.h"

#include "Component.h"

/** @brief	A Bullet collision shape. */
class CollisionShape : public Component
{
public:

	~CollisionShape();

	/**
	 @brief	Generates a static mesh shape.
	
	 @return	True if it succeeds, false if it fails.
	 */
	bool generateStaticMeshShape();

	/**
	 @brief	Generates a convex mesh shape.
	
	 @return	True if it succeeds, false if it fails.
	 */
	bool generateConvexMeshShape();

	/**
	 @brief	Generates a box shape. Uses AABB to calculate.
	
	 @return	True if it succeeds, false if it fails.
	 */
	bool generateBoxShape();

	/**
	 @brief	Gets the shape.
	
	 @return	Null if it fails, else the shape.
	 */
	btCollisionShape* getShape();
private:

	/** @brief	The shape. */
	btCollisionShape* shape;
};