#pragma once

#include "Component.h"

#include "../misc/Utility.h"

class ExpiryTimer : public Component
{
public:
	ExpiryTimer();

	virtual ~ExpiryTimer();

	virtual void onUpdate();

	void startExpiryTimer(float length);

protected:
	Utility::SimpleTimer* timer;
};