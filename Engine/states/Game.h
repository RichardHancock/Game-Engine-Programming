#pragma once

#include "State.h"

#include <SDL.h>

#include "../Shader.h"
#include "../components/Camera.h"
#include "../GameModel.h"
#include "../Audio.h"
#include "../misc/Vec3.h"

/** @brief	The main game state. */
class Game : public State
{
public:

	/**
	@brief Create the State.
	 */
	Game();

	/** @brief	Destructor. */
	virtual ~Game();

	/**
	@brief Handles any events such as keyboard/mouse input
	*/
	virtual bool eventHandler();

	/**
	@brief Update any internal values
	@param dt delta time
	*/
	virtual void update(float dt);

	/**
	@brief Render any sprites relevant to the state
	*/
	virtual void render();

private:

	/**
	 @brief	Handles all movement of objects by checking if a button or key has been pressed.
	
	 @param	dt	Deltatime.
	 */
	void movementControls(float dt);

	/** @brief	true if currently controlling camera. */
	bool controllingCamera;

};