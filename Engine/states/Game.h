#pragma once

#include "State.h"

#include <SDL.h>

#include "../Shader.h"
#include "../components/Camera.h"
#include "../GameModel.h"
#include "../Audio.h"
#include "../misc/Vec3.h"
#include "../ui/UI.h"

class Game : public State
{
public:

	/**
	@brief Create the State.
	 */
	Game();

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
	void cameraControls(float dt);

	void collisionResponse();

	//UIElement* ui;

	//UITextElement* text;

	//TTF_Font* font;

};