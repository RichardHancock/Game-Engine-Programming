#include <SDL.h>
#include <string>
#include "../Platform.h"
#include "../ResourceManager.h"
#include "../InputManager.h"

#pragma once
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
	
	@param [in,out] manager		    Pointer to the current state manager.
	@param [in,out] platform	    Pointer to platform.
	@param [in,out] resourceManager If non-null, manager for resources.
	@param [in,out] inputManager    If non-null, manager for input.
	 */
	State(StateManager* manager, Platform* platform, 
		ResourceManager* resourceManager, InputManager* inputManager);

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
	///Pointer to state manager
	StateManager* stateManager;

	///Platform
	Platform* platform;

	///Resource Manager
	ResourceManager* resourceManager;

	///Input Manager
	InputManager* inputManager;

	///This states name
	std::string stateName;
};