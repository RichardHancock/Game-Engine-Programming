#pragma once

#include "State.h"

#include <SDL.h>

#include "../Shader.h"
#include "../components/Camera.h"
#include "../GameModel.h"
#include "../Audio.h"
#include "../misc/Vec3.h"
#include "../Socket.h"
#include "../mobileUI/MobileGameUI.h"
#include "../CubeMap.h"

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
	*/
	virtual void update();

	/**
	@brief Render any sprites relevant to the state
	*/
	virtual void render();

private:

	/**
	 @brief	Handles all movement of objects by checking if a button or key has been pressed.
	 */
	void movementControls();


	//void mobileUIUpdate();

	//void mobileUIEventQueue();

	//void toggleQRVisiblity();

	/** @brief	The sky box's cube map. */
	std::shared_ptr<CubeMap> skyBoxCubeMap;

	/** @brief	The socket (not Used currently). */
	Socket* socket;


	/*
	//MOBILE UI DEMO BELOW
	MobileGameUI* mobileUI;

	std::shared_ptr<Texture> qrTexture;

	bool qrGenerated;

	struct SentValues
	{
		glm::vec3 rotation;

		float health;
		float shield;
	} sentValues;


	float shipHealth;
	float shipShield;
	*/
};