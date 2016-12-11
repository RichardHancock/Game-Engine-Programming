#pragma once

#include <SDL.h>
#include <string>

#include "../Platform.h"
#include "../ResourceManager.h"
#include "../input/InputManager.h"

class StateManager;

/**
@class State
@brief A game state that contains one screen of the game such as a Menu or the game itself
*/
class State
{
public:

	/**
	@brief Create the State.
	 */
	State();

	virtual ~State() {}

	/**
	@brief Handles any events such as keyboard/mouse input
	@return Continue the game
	*/
	virtual bool eventHandler() = 0;

	/**
	@brief Update any internal values
	@param dt delta time
	*/
	virtual void update(float dt) = 0;

	/**
	@brief Render any sprites relevant to the state
	*/
	virtual void render() = 0;

	/**
	@brief Get this states name
	@return std::string - Name
	*/
	std::string getStateName() { return stateName; }
protected:

	///This states name
	std::string stateName;
};