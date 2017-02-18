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

	/** @brief	true if currently controlling camera. */
	bool controllingCamera;

	void mobileUIUpdate();

	std::shared_ptr<GameModel> heightmap;

	Socket* socket;

	MobileGameUI* mobileUI;

	std::shared_ptr<Texture> qrTexture;

	bool qrGenerated;
};