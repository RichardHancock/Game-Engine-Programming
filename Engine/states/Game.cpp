#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../misc/Log.h"
#include "../misc/Utility.h"
#include "../Transform.h"
#include "../MeshComponent.h"
#include "../Material.h"
#include "../MeshRenderer.h"

Game::Game(StateManager* manager, Platform* platform)
	: State(manager, platform)
{
	stateName = "Game";

	testSFX = ResourceManager::getAudio("POP Echo Bouncer 01.wav", false);

	//ui = new UIElement(Vec2(-1.0f, 1.0f), Vec2(0.3f, -0.2f));
	//ui->addTexture(ResourceManager::getTexture("uiTest.png"), "gSampler");

	//font = TTF_OpenFont("resources/fonts/OpenSans-Regular.ttf", 32);
	//text = new UITextElement(Vec2(0.0f), Vec2(0.5f, -0.5f ), "Testing", Colour(128), font);
	//text->addTexture(ResourceManager::getTexture("uiTest.png"), "gSampler");

	hideGUI = false;

	//Camera
	cameraObj = std::make_shared<GameObject>("Camera");
	auto camTransform = cameraObj->addComponent<Transform>("Transform").lock();
	camTransform->setPostion(glm::vec3(0, -3, -10));
	camTransform->setRotation(glm::vec3(0, 0, 0));
	camTransform->setScale(glm::vec3(1));

	auto cameraComponent = cameraObj->addComponent<Camera>("Camera");
	ResourceManager::engineState->currentCamera = cameraComponent;

	//Game Object
	gameO = std::make_shared<GameObject>("testing");
	auto transform = gameO->addComponent<Transform>("Transform").lock();
	transform->setPostion(glm::vec3(0, 0, 0));
	transform->setScale(glm::vec3(1));

	gameO->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("Viper-mk-IV-fighter.obj"));

	//Material
	//std::string shaderDir = ResourceManager::shaderDir;
	//
	//material = std::make_shared<Material>(shaderDir + "vertexNormal.shader", shaderDir + "fragmentNormal.shader");
	//material->addTexture("diffuseMap", ResourceManager::getTexture("barrel.png"));
	//material->addTexture("normalMap", ResourceManager::getTexture("barrelNormal.png"));

	gameO->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("Viper-mk-IV-fighter.obj")
	);

	gameO->onAwake();
	cameraObj->onAwake();
}

Game::~Game()
{
	//TTF_CloseFont(font);

	//delete ui;
	//delete text;
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
	//Sound Test
	if (InputManager::wasKeyReleased(SDLK_p))
	{
		testSFX.lock()->play(0, 0);
	}

	cameraControls(dt);

	InputManager::printDebugInfo();

	if (InputManager::wasKeyPressed(SDLK_g))
		hideGUI = !hideGUI;


	gameO->getComponent<Transform>("Transform").lock()->rotate(glm::vec3(0.0f, 1.0f * dt, 0.0f));


	gameO->onUpdate();
	cameraObj->onUpdate();
}

void Game::render()
{
	gameO->onRender();

	//UI
	/*if (!hideGUI)
	{
		ui->draw(shader2D);
		text->draw(shader2D);
	}*/
}

void Game::cameraControls(float dt)
{
	std::shared_ptr<Transform> camera = cameraObj->getComponent<Transform>("Transform").lock();

	float speed = 50.0f;

	//move along camera along x
	if (InputManager::isKeyHeld(SDLK_a))
	{
		camera->translate(glm::vec3(speed * dt, 0, 0));
	}
	else if (InputManager::isKeyHeld(SDLK_d))
	{
		camera->translate(glm::vec3(-speed * dt, 0, 0));
	}

	//move camera along y
	if (InputManager::isKeyHeld(SDLK_q))
	{
		camera->translate(glm::vec3(0, speed * dt, 0));
	}
	else if (InputManager::isKeyHeld(SDLK_e))
	{
		camera->translate(glm::vec3(0, -speed * dt, 0));
	}

	//move camera along z
	if (InputManager::isKeyHeld(SDLK_w))
	{
		camera->translate(glm::vec3(0, 0, speed * dt));
	}
	if (InputManager::isKeyHeld(SDLK_s))
	{
		camera->translate(glm::vec3(0, 0, -speed * dt));
	}

	float speedRadians = Utility::convertAngleToRadian(speed * dt);

	//rotate along camera along x
	if (InputManager::isKeyHeld(SDLK_UP))
	{
		camera->rotate(glm::vec3(-speedRadians, 0, 0));
	}
	else if (InputManager::isKeyHeld(SDLK_DOWN))
	{
		camera->rotate(glm::vec3(speedRadians, 0, 0));
	}
	//rotate camera along y
	if (InputManager::isKeyHeld(SDLK_LEFT))
	{
		camera->rotate(glm::vec3(0, -speedRadians, 0));
	}
	if (InputManager::isKeyHeld(SDLK_RIGHT))
	{
		camera->rotate(glm::vec3(0, speedRadians, 0));
	}
	//rotate camera along z
	if (InputManager::isKeyHeld(SDLK_k))
	{
		camera->rotate(glm::vec3(0, 0, -speedRadians));
	}
	if (InputManager::isKeyHeld(SDLK_l))
	{
		camera->rotate(glm::vec3(0, 0, speedRadians));
	}

	//reset camera to 0,0,0
	if (InputManager::wasKeyReleased(SDLK_SPACE))
	{
		camera->setRotation(glm::vec3(0.0f));
		camera->setPostion(glm::vec3(0, -3, -10));
	}

}