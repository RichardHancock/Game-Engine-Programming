#pragma once

#include "Component.h"

#include "../misc/Utility.h"

/** @brief	A controller for handling ship movement and controls. */
class ShipController : public Component
{
public:
	/** @brief	Default constructor. */
	ShipController();

	/** @brief	Destructor. */
	virtual ~ShipController();


	/** @brief	Executes the update action. */
	virtual void onUpdate();

protected:
	/** @brief	Movement controls. */
	void movementControls();
	
	/** @brief	Spawn and fire rocket. */
	void fireRocket();

	/** @brief	The reload timer for the rockets. */
	Utility::SimpleTimer reloadTimer;

};