#include "State.h"
#include "../Shader.h"
#include "../Camera.h"

#include "../GameModel.h"
#include "../Audio.h"
#include "../misc/Vec3.h"
#include <SDL.h>
#include "../ui/UI.h"

class Game : public State
{
public:

	/**
	@brief Create the State.
	
	@param [in,out] manager		    Pointer to the current state manager.
	@param [in,out] platform	    Platform containing OS dependent data.
	@param [in,out] resourceManager If non-null, manager for resources.
	@param [in,out] inputManager    If non-null, manager for input.
	 */
	Game(StateManager* manager, Platform* platform, 
		ResourceManager* resourceManager, InputManager* inputManager);

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

	Shader* standardShader;
	Shader* shader2D;

	Camera* camera;

	GameModel* test;
	GameModel* bowl;

	Audio* testSFX;

	UIElement* ui;

	UITextElement* text;

	TTF_Font* font;

	bool hideGUI;
};