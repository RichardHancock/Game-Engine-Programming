#pragma once

#include "Component.h"

#include "../misc/Utility.h"

/** @brief	An expiry timer (Once it runs out attached game object will be destroyed. */
class ExpiryTimer : public Component
{
public:
	/** @brief	Default constructor. */
	ExpiryTimer();

	/** @brief	Destructor. */
	virtual ~ExpiryTimer();

	/** @brief	Executes the update action. */
	virtual void onUpdate();

	/**
	 @brief	Starts expiry timer.
	
	 @param	length	The length.
	 */
	void startExpiryTimer(float length);

protected:
	/** @brief	The timer. */
	Utility::SimpleTimer* timer;
};