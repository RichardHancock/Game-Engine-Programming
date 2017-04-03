#pragma once

#include "Component.h"

/** @brief	An explosive collision handler (On collision trigger explosion bubble). */
class ExplosiveCollision : public Component
{
public:

	/** @brief	Destructor. */
	virtual ~ExplosiveCollision();

	/**
	 @brief	Executes the collision action.
	
	 @param	objAName		Name of the object a.
	 @param	objBName		Name of the object b.
	 @param	contactPoint	The contact point.
	 */
	virtual void onCollision(std::string objAName, std::string objBName, glm::vec3 contactPoint);

protected:
};