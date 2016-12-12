#pragma once

#include <memory>

#include "Collider.h"

//Ref: Inspired by Karsten's with several changes

/** @brief	A collision tester for spheres against other objects. */
class SphereCollider : public Collider
{
public:
	/** @brief	Default constructor. */
	SphereCollider();

	/** @brief	Executes the update action. */
	virtual void onUpdate();
	/** @brief	Executes the awake action. */
	virtual void onAwake();

	/**
	 @brief	Query if this SphereCollider is colliding.
	
	 @return	true if colliding, false if not.
	 */
	bool isColliding();

private:
	/** @brief	true if sphere is colliding. */
	bool colliding;

	/**
	 @brief	Checks if the other sphere intersects with the sphere.
	
	 @param	other	The other sphere.
	
	 @return	true if it succeeds, false if it fails.
	 */
	bool intersectSphere(std::weak_ptr<SphereCollider> other);

	/**
	 @brief Checks if the AABB intersects with the sphere.
	
	 @param	other	The AABB.
	
	 @return	true if it succeeds, false if it fails.
	 */
	bool intersectAABB(AABB other);
};
