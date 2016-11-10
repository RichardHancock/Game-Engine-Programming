#include "Game.h"
#include "../misc/Log.h"
#include "../misc/Utility.h"
#include <glm/gtc/matrix_transform.hpp>

Game::Game(StateManager* manager, Platform* platform)
	: State(manager, platform)
{
	stateName = "Game";

	std::string shaderPath = ResourceManager::shaderDir;
	standardShader = new Shader(
		shaderPath + "vertex.shader", 
		shaderPath + "fragment.shader"
	);

	shader2D = new Shader(
		shaderPath + "2D vertex.shader",
		shaderPath + "2D fragment.shader"
	);

	//camera = new Camera();
	
	/*if (SDL_SetRelativeMouseMode(SDL_TRUE) == -1)
	{
		Log::logW("Bugger, mouse not happy");
	}*/

	//test = ResourceManager::getModel("barrel.obj", ResourceManager::getTexture("barrel.png"));
	//bowl = ResourceManager::getModel("bowl.obj", ResourceManager::getTexture("bowl.png"));

	testSFX = ResourceManager::getAudio("POP Echo Bouncer 01.wav", false);

	//ui = new UIElement(Vec2(-1.0f, 1.0f), Vec2(0.3f, -0.2f));
	//ui->addTexture(ResourceManager::getTexture("uiTest.png"), "gSampler");

	//font = TTF_OpenFont("resources/fonts/OpenSans-Regular.ttf", 32);
	//text = new UITextElement(Vec2(0.0f), Vec2(0.5f, -0.5f ), "Testing", Colour(128), font);
	//text->addTexture(ResourceManager::getTexture("uiTest.png"), "gSampler");

	hideGUI = false;

	//camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
}

Game::~Game()
{
	//TTF_CloseFont(font);

	//delete ui;
	//delete text;

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
			InputManager::processKeyEvent(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			InputManager::processMouseEvent(e);

			break;

		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
			InputManager::processGameControllerEvent(e);
			break;
		}
	}

	if (InputManager::wasKeyReleased(SDLK_ESCAPE))
	{
		return true;
	}

	return false;
}

void Game::update(float dt)
{
	
	//camera->updateViewMat(glm::vec3(InputManager::getMouseDirection().x, 0.0f, 0.0f);
		//InputManager::getMouseDirection().y, InputManager::getMouseWheelDirection().y));

	//Sound Test
	if (InputManager::wasKeyReleased(SDLK_p))
	{
		testSFX.lock()->play(0, 0);
	}

	cameraControls(dt);

	//Mouse Tests
	if (InputManager::wasMouseButtonPressed(SDL_BUTTON_LEFT)) {
		Log::logI("Mouse Pos: " + Utility::vec2ToString(InputManager::getMousePos()));
	}
	if (InputManager::wasMouseButtonPressed(SDL_BUTTON_MIDDLE)) {
		Log::logI("Mouse Dir: " + Utility::vec2ToString(InputManager::getMouseDirection()));
	}
	if (InputManager::wasMouseButtonPressed(SDL_BUTTON_RIGHT)) {
		Log::logI("Mouse wheel: " + Utility::vec2ToString(InputManager::getMouseWheelDirection()));
	}
	
	//Controller Tests
	if (InputManager::wasControllerButtonPressed(0, Controller::A))
	{
		Log::logI("LeftAxis: " +
			Utility::vec2ToString(InputManager::getControllerAxis2D(0, Controller::LeftStick)));
		Log::logI("RightAxis: " +
			Utility::vec2ToString(InputManager::getControllerAxis2D(0, Controller::RightStick)));
		Log::logI("LeftTrigger: " +
			Utility::floatToString(InputManager::getControllerAxis1D(0, Controller::LeftTrigger)));
		Log::logI("RightTrigger: " +
			Utility::floatToString(InputManager::getControllerAxis1D(0, Controller::RightTrigger)));
	}

	if (InputManager::wasControllerButtonPressed(0, Controller::RIGHTSHOULDER))
	{
		InputManager::playControllerRumble(0, 1.0f, 2000);
	}

	if (InputManager::wasKeyPressed(SDLK_g))
		hideGUI = !hideGUI;
}

void Game::render()
{
	//glm::mat4 projection = camera->getProjMatrix();
	//glm::mat4 view = camera->getViewMatrix();
	
	glm::mat4 model;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -2.0f, -10.0f));
	model = glm::scale(model, glm::vec3(1.0f));

	//test->draw(model, view, projection, standardShader);
	
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, -15.0f));
	//bowl->draw(model, view, projection, standardShader);

	//UI
	/*if (!hideGUI)
	{
		ui->draw(shader2D);
		text->draw(shader2D);
	}*/
}

void Game::cameraControls(float dt)
{
	/*
	//move along camera along x
	if (InputManager::isKeyHeld(SDLK_a))
	{
		camera->moveCameraAlongX(-100.0f * dt);
	}
	if (InputManager::isKeyHeld(SDLK_d))
	{
		camera->moveCameraAlongX(100.0f * dt);
	}
	//move camera along y
	if (InputManager::isKeyHeld(SDLK_q))
	{
		camera->moveCameraAlongY(-100.0f * dt);
	}
	if (InputManager::isKeyHeld(SDLK_e))
	{
		camera->moveCameraAlongY(100.0f * dt);
	}
	//move camera along z
	if (InputManager::isKeyHeld(SDLK_w))
	{
		camera->moveCameraAlongZ(-100.0f * dt);
	}
	if (InputManager::isKeyHeld(SDLK_s))
	{
		camera->moveCameraAlongZ(100.0f * dt);
	}

	//rotate along camera along x
	if (InputManager::isKeyHeld(SDLK_UP))
	{
		camera->rotateCameraAlongX(Utility::convertAngleToRadian(-100.0f * dt));
	}
	if (InputManager::isKeyHeld(SDLK_DOWN))
	{
		camera->rotateCameraAlongX(Utility::convertAngleToRadian(100.0f * dt));
	}
	//rotate camera along y
	if (InputManager::isKeyHeld(SDLK_LEFT))
	{
		camera->rotateCameraAlongY(Utility::convertAngleToRadian(-100.0f * dt));
	}
	if (InputManager::isKeyHeld(SDLK_RIGHT))
	{
		camera->rotateCameraAlongY(Utility::convertAngleToRadian(100.0f * dt));
	}
	//rotate camera along z
	if (InputManager::isKeyHeld(SDLK_k))
	{
		camera->rotateCameraAlongZ(Utility::convertAngleToRadian(-100.0f * dt));
	}
	if (InputManager::isKeyHeld(SDLK_l))
	{
		camera->rotateCameraAlongZ(Utility::convertAngleToRadian(100.0f * dt));
	}

	//reset camera to 0,0,0
	if (InputManager::wasKeyReleased(SDLK_SPACE))
	{
		camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	*/
}