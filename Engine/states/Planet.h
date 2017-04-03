#pragma once

#include "State.h"

class PlanetState : public State
{
public:
	/**
	@brief Create the State.
	*/
	PlanetState();

	/** @brief	Destructor. */
	virtual ~PlanetState();

	/**
	@brief Handles any events such as keyboard/mouse input
	*/
	virtual bool eventHandler();

	/**
	@brief Update any internal values
	*/
	virtual void update();

	/**
	@brief Render any sprites relevant to the state
	*/
	virtual void render();
protected:

	std::shared_ptr<CubeMap> skyBoxCubeMap;
	std::shared_ptr<GameModel> heightmap;
};