#pragma once

#include "State.h"
#include <vector>
#include <SDL.h>

/**
@class StateManager
@brief Manages the creation, switching and function forwarding for States
*/
class StateManager
{
public:
	
	static void cleanup();

	/**
	@brief Add a state on top of the previous state
	@param state The new state
	*/
	static void addState(std::shared_ptr<State> state);

	/**
	@brief Resets the Timer class otherwise conflicts happen
	*/
	static void prepareToChangeState();

	/**
	@brief Change to a new state while deleting all previous states
	@param state The new State
	*/
	static void changeState(std::shared_ptr<State> state);

	/**
	@brief Deletes the most recent state 
	*/
	static void popLastState();


	//Pass through functions that call the states internal functions

	///Calls the event handler in the current state
	static bool eventHandler();

	/**
	@brief Calls the update in the current state
	@param dt delta time
	*/
	static void update(float dt);

	///Calls the render for all loaded states
	static void render();

private:
	/**
	@brief Deletes all currently loaded states
	*/
	static void clearStates();

	///All the currently loaded states
	static std::vector<std::shared_ptr<State>> states;

};