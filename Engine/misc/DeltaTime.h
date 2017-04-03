#pragma once

/** @brief	A delta time manager. */
class DeltaTime
{
public:

	/**
	 @brief	Gets Delta Time.
	
	 @return	The Delta Time.
	 */
	static float getDT();

	/** @brief	Initialises this DeltaTime. */
	static void init();

	/** @brief	Updates this DeltaTime. */
	static void update();

private:

	/** @brief	The delta time. */
	static float deltaTime;

	/** @brief	The last time. */
	static unsigned int lastTime;
};