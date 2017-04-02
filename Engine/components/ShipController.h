#pragma once

#include "Component.h"

#include "../misc/Utility.h"

class ShipController : public Component
{
public:
	ShipController();

	virtual ~ShipController();


	virtual void onUpdate();

protected:
	void movementControls();
	
	void fireRocket();

	Utility::SimpleTimer reloadTimer;

};