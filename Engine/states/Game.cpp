#include "Game.h"
#include "../misc/Log.h"
#include "../misc/Utility.h"
#include <glm/gtc/matrix_transform.hpp>

Game::Game(StateManager* manager, Platform* platform, 
	ResourceManager* resourceManager, InputManager* inputManager)
	: State(manager, platform, resourceManager, inputManager)
{
	stateName = "Game";

	std::string shaderPath = resourceManager->shaderDir;
	standardShader = new Shader(
		shaderPath + "vertex.shader", 
		shaderPath + "fragment.shader"
	);

	shader2D = new Shader(
		shaderPath + "2D vertex.shader",
		shaderPath + "2D fragment.shader"
	);

	camera = new Camera();
	
	/*if (SDL_SetRelativeMouseMode(SDL_TRUE) == -1)
	{
		Log::logW("Bugger, mouse not happy");
	}*/

	test = resourceManager->getModel("barrel.obj", resourceManager->getTexture("barrel.png"));
	bowl = resourceManager->getModel("bowl.obj", resourceManager->getTexture("bowl.png"));

	testSFX = resourceManager->getAudio("POP Echo Bouncer 01.wav", false);

	ui = new UIElement(Vec2(-1.0f, 1.0f), Vec2(0.3f, -0.2f));
	ui->addTexture(resourceManager->getTexture("uiTest.png"), "gSampler");

	TTF_Font* font = TTF_OpenFont("resources/fonts/OpenSans-Regular.ttf", 32);
	text = new UITextElement(Vec2(0.0f), Vec2(0.5f, -0.5f ), "Testing", Colour(128), font);
	text->addTexture(resourceManager->getTexture("uiTest.png"), "gSampler");

	hideGUI = false;

	camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
}

Game::~Game()
{
	test->freeResourceInstance();
	bowl->freeResourceInstance();
	testSFX->freeResourceInstance();

	delete ui;
	delete text;

	delete camera;
	delete standardShader;
}

bool Game::eventHandler()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return true;
			break;

		case SDL_KEYUP:
		case SDL_KEYDOWN:
			inputManager->processKeyEvent(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			inputManager->processMouseEvent(e);

			break;

		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
			inputManager->processGameControllerEvent(e);
			break;
		}
	}

	if (inputManager->wasKeyReleased(SDLK_ESCAPE))
	{
		return true;
	}

	return false;
}

void Game::update(float dt)
{
	
	//camera->updateViewMat(glm::vec3(inputManager->getMouseDirection().x, 0.0f, 0.0f);
		//inputManager->getMouseDirection().y, inputManager->getMouseWheelDirection().y));

	//Sound Test
	if (inputManager->wasKeyReleased(SDLK_p))
	{
		testSFX->play(0, 0);
	}

	cameraControls(dt);

	//Mouse Tests
	if (inputManager->wasMouseButtonPressed(SDL_BUTTON_LEFT)) {
		Log::logI("Mouse Pos: " + Utility::vec2ToString(inputManager->getMousePos()));
	}
	if (inputManager->wasMouseButtonPressed(SDL_BUTTON_MIDDLE)) {
		Log::logI("Mouse Dir: " + Utility::vec2ToString(inputManager->getMouseDirection()));
	}
	if (inputManager->wasMouseButtonPressed(SDL_BUTTON_RIGHT)) {
		Log::logI("Mouse wheel: " + Utility::vec2ToString(inputManager->getMouseWheelDirection()));
	}
	
	//Controller Tests
	if (inputManager->wasControllerButtonPressed(0, Controller::A))
	{
		Log::logI("LeftAxis: " +
			Utility::vec2ToString(inputManager->getControllerAxis2D(0, Controller::LeftStick)));
		Log::logI("RightAxis: " +
			Utility::vec2ToString(inputManager->getControllerAxis2D(0, Controller::RightStick)));
		Log::logI("LeftTrigger: " +
			Utility::floatToString(inputManager->getControllerAxis1D(0, Controller::LeftTrigger)));
		Log::logI("RightTrigger: " +
			Utility::floatToString(inputManager->getControllerAxis1D(0, Controller::RightTrigger)));
	}

	if (inputManager->wasControllerButtonPressed(0, Controller::RIGHTSHOULDER))
	{
		inputManager->playControllerRumble(0, 1.0f, 2000);
	}

	if (inputManager->wasKeyPressed(SDLK_g))
		hideGUI = !hideGUI;
}

void Game::render()
{
	glm::mat4 projection = camera->getProjMatrix();
	glm::mat4 view = camera->getViewMatrix();
	
	glm::mat4 model;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -2.0f, -10.0f));
	model = glm::scale(model, glm::vec3(1.0f));

	test->draw(model, view, projection, standardShader);
	
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, -15.0f));
	bowl->draw(model, view, projection, standardShader);

	//UI
	if (!hideGUI)
	{
		ui->draw(shader2D);
		text->draw(shader2D);
	}
}

void Game::cameraControls(float dt)
{
	//move along camera along x
	if (inputManager->isKeyHeld(SDLK_a))
	{
		camera->moveCameraAlongX(-100.0f * dt);
	}
	if (inputManager->isKeyHeld(SDLK_d))
	{
		camera->moveCameraAlongX(100.0f * dt);
	}
	//move camera along y
	if (inputManager->isKeyHeld(SDLK_q))
	{
		camera->moveCameraAlongY(-100.0f * dt);
	}
	if (inputManager->isKeyHeld(SDLK_e))
	{
		camera->moveCameraAlongY(100.0f * dt);
	}
	//move camera along z
	if (inputManager->isKeyHeld(SDLK_w))
	{
		camera->moveCameraAlongZ(-100.0f * dt);
	}
	if (inputManager->isKeyHeld(SDLK_s))
	{
		camera->moveCameraAlongZ(100.0f * dt);
	}

	//rotate along camera along x
	if (inputManager->isKeyHeld(SDLK_UP))
	{
		camera->rotateCameraAlongX(Utility::convertAngleToRadian(-100.0f * dt));
	}
	if (inputManager->isKeyHeld(SDLK_DOWN))
	{
		camera->rotateCameraAlongX(Utility::convertAngleToRadian(100.0f * dt));
	}
	//rotate camera along y
	if (inputManager->isKeyHeld(SDLK_LEFT))
	{
		camera->rotateCameraAlongY(Utility::convertAngleToRadian(-100.0f * dt));
	}
	if (inputManager->isKeyHeld(SDLK_RIGHT))
	{
		camera->rotateCameraAlongY(Utility::convertAngleToRadian(100.0f * dt));
	}
	//rotate camera along z
	if (inputManager->isKeyHeld(SDLK_k))
	{
		camera->rotateCameraAlongZ(Utility::convertAngleToRadian(-100.0f * dt));
	}
	if (inputManager->isKeyHeld(SDLK_l))
	{
		camera->rotateCameraAlongZ(Utility::convertAngleToRadian(100.0f * dt));
	}

	//reset camera to 0,0,0
	if (inputManager->wasKeyReleased(SDLK_SPACE))
	{
		camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}