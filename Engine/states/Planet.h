#pragma once

#include "State.h"

/** @brief	A planet. */
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

	/** @brief	The sky box's cube map. */
	std::shared_ptr<CubeMap> skyBoxCubeMap;

	/** @brief	The heightmap. */
	std::shared_ptr<GameModel> heightmap;
};