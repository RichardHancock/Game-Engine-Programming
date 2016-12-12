#pragma once

#include "Component.h"
#include "../misc/AABB.h"
#include "Transform.h"

//Ref: Inspired by Karsten's with several changes

/** @brief	A collider base class component that provides to framework for collision testing. */
class Collider : public Component
{
public:
	/** @brief	Default constructor. */
	Collider();

	/** @brief	Destructor. */
	virtual ~Collider();

	/**
	 @brief	Gets the AABB.
	
	 @return	The bounds.
	 */
	AABB getBounds();

	/** @brief	Executes the awake action. */
	virtual void onAwake();

	/**
	 @brief	Gets the transform component of the parent.
	 Shortcut as all gameobjects have a transform component
	 @return	The transform.
	 */
	std::shared_ptr<Transform> getTransform();

protected:

	
	/** @brief	Has the AABB been stored in this component or does the mesh component need to be queried */
	bool boundsFetched;

private:
	/** @brief	Updates the AABB bounds. */
	void updateBounds();
	/** @brief	The Axis Aligned Bounding Box. */
	AABB bounds;
};