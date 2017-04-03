#pragma once

#include "Component.h"

#include <glm/glm.hpp>

#include "../internal/dependencies/btBulletDynamicsCommon.h"
#include "CollisionShape.h"

/** @brief	A Bullet rigid body. */
class RigidBody : public Component
{
public:
	/** @brief	Default constructor. */
	RigidBody();

	/** @brief	Destructor. */
	~RigidBody();

	/**
	 @brief	Sets the position.
	
	 @param	newPos	The new position.
	 */
	void setPosition(glm::vec3 newPos);

	/**
	 @brief	Sets the rotation.
	
	 @param [in,out]	newRotation	The new rotation.
	 */
	void setRotation(btQuaternion& newRotation);

	/**
	 @brief	Sets a damping (How quick speed will bleed off, 1.0f being the fastest).
	
	 @param	linearDamping 	The linear damping.
	 @param	angularDamping	The angular damping.
	 */
	void setDamping(float linearDamping, float angularDamping);

	/**
	 @brief	Sets a restitution (How Bouncy or glitchy or nothing at all, pure magic setting).
	
	 @param	value	The value.
	 */
	void setRestitution(float value);

	/** @brief	Executes the awake action. */
	virtual void onAwake();

	/**
	 @brief	Applies the force described by newForce.
	
	 @param	newForce	The new force.
	 */
	void applyForce(glm::vec3 newForce);

	/**
	 @brief	Initialise the RigidBody with the passed in properties
	
	 @param	weight 	The weight.
	 @param	inertia	The inertia.
	 */
	void init(float weight, glm::vec3 inertia);

private:
	/** @brief	The rigid body. */
	btRigidBody* rigidBody;

	/** @brief	State of the motion. */
	btMotionState* motionState;
};