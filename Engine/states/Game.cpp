#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../misc/Log.h"
#include "../misc/Utility.h"
#include "../components/Transform.h"
#include "../components/MeshComponent.h"
#include "../Material.h"
#include "../components/MeshRenderer.h"
#include "../misc/GameVariables.h"
#include "../components/SphereCollider.h"
#include "../components/Light.h"

Game::Game()
{
	stateName = "Game";

	controllingCamera = true;

	//Camera
	auto cameraObj = GameObject::create("Camera").lock();
	auto camTransform = cameraObj->addComponent<Transform>("Transform").lock();
	camTransform->setPostion(glm::vec3(0, 0, 40));
	camTransform->setRotation(glm::vec3(0, 0, 0));
	camTransform->setScale(glm::vec3(1));

	auto cameraComponent = cameraObj->addComponent<Camera>("Camera");
	GameVariables::data->currentCamera = cameraComponent;

	//Game Object
	auto gameO = GameObject::create("fighter").lock();
	auto transform = gameO->addComponent<Transform>("Transform").lock();
	transform->setPostion(glm::vec3(0.0f, 0.0f, 5.0f));
	transform->setScale(glm::vec3(1));

	gameO->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("dark_fighter_6.obj"));

	gameO->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("dark_fighter_6.obj")
	);

	auto ship = GameObject::create("ship").lock();
	auto transform2 = ship->addComponent<Transform>("Transform").lock();
	transform2->setPostion(glm::vec3(0.0f, 30.0f, 10.0f));
	transform2->setScale(glm::vec3(1));

	ship->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("space_frigate_6.obj"));

	ship->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("space_frigate_6.obj")
	);

	auto earth = GameObject::create("earth").lock();
	auto transform3 = earth->addComponent<Transform>("Transform").lock();
	transform3->setPostion(glm::vec3(-20.0f, 50.0f, -150.0f));
	transform3->setScale(glm::vec3(50));

	earth->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("Earth.obj"));

	earth->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("Earth.obj")
	);

	auto flatPlane = GameObject::create("bg").lock();
	auto transform4 = flatPlane->addComponent<Transform>("Transform").lock();
	transform4->setPostion(glm::vec3(-20.0f, 50.0f, -150.0f));
	transform4->setRotation(glm::vec3(Utility::HALF_PI, 0.0f, 0.0f));
	transform4->setScale(glm::vec3(50));

	flatPlane->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("flatPlane.obj"));

	flatPlane->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("flatPlane.obj")
	);


	ResourceManager::createMaterial("blue", ResourceManager::getTexture("blue.png"),
		"vertexMat.shader", "fragmentMat.shader");
	ResourceManager::createMaterial("red", ResourceManager::getTexture("red.png"),
		"vertexMat.shader", "fragmentMat.shader");


	//ALL BELOW ARE LOADED WITH MY OWN OBJ LOADER
	//spheres (Loaded using my own OBJ loader)
	auto light = GameObject::create("light").lock();
	auto lightT = light->addComponent<Transform>("Transform").lock();
	lightT->setPostion(glm::vec3(-50.0f, 10.0f, -5.0f));

	light->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("bowl.obj", false));
	
	light->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterial(
		ResourceManager::getMaterial("red", 0, false)
	);
	light->addComponent<SphereCollider>("SphereCollider");
	GameVariables::data->currentLight = light;
	
	light->addComponent<Light>("Light").lock()->init(
		1.0f,
		0.027f,
		0.0028f,
		glm::vec3(0.9f, 0.0f, 0.0f),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(0.5f)
	);

	
	//Need to load second as I currently don't support loading the material individually.
	auto sphere = GameObject::create("sphere").lock();
	auto sphereT = sphere->addComponent<Transform>("Transform").lock();
	sphereT->setPostion(glm::vec3(-40.0f, 0.0f, -5.0f));

	sphere->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("bowl.obj", false));

	sphere->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterial(
		ResourceManager::getMaterial("red" , 0, false)
	);
	sphere->addComponent<SphereCollider>("SphereCollider");



	gameO->onAwake();
	ship->onAwake();
	earth->onAwake();
	flatPlane->onAwake();
	light->onAwake();
	sphere->onAwake();
	cameraObj->onAwake();
}

Game::~Game()
{
	
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
	movementControls(dt);

	InputManager::printDebugInfo();


	if (InputManager::wasKeyPressed(SDLK_1) || InputManager::wasControllerButtonPressed(0,Controller::Button::B))
	{
		ResourceManager::getAudio("Item Place.wav", false).lock()->play(0,0);
	}
	if (InputManager::wasKeyPressed(SDLK_c) || InputManager::wasControllerButtonPressed(0, Controller::Button::Y))
	{
		controllingCamera = !controllingCamera;
	}

	
	//reset scene
	if (InputManager::wasKeyReleased(SDLK_SPACE) || InputManager::wasControllerButtonPressed(0, Controller::Button::B))
	{
		std::shared_ptr<Transform> camera = GameVariables::data->currentCamera.lock()->getGameObject().lock()
			->getComponent<Transform>("Transform").lock();

		camera->setRotation(glm::vec3(0.0f));
		camera->setPostion(glm::vec3(0, -3, -10));

		GameVariables::data->gameObjs["light"]->getComponent<Transform>().lock()->setPostion(glm::vec3(-50.0f, 10.0f, -5.0f));
		GameVariables::data->gameObjs["sphere"]->getComponent<Transform>().lock()->setPostion(glm::vec3(-40.0f, 0.0f, -5.0f));
	}
	
	collisionResponse();

	GameVariables::data->gameObjs["fighter"]->getComponent<Transform>("Transform").lock()->rotate(glm::vec3(0.0f, 1.0f * dt, 0.0f));

	for (auto object : GameVariables::data->gameObjs)
	{
		object.second->onUpdate();
	}
}

void Game::render()
{
	for (auto object : GameVariables::data->gameObjs)
	{
		object.second->onRender();
	}

}

void Game::movementControls(float dt)
{
	//If camera control mode get pointer to camera transform, else get the light's transform instead
	std::shared_ptr<Transform> object = (controllingCamera ? 
		GameVariables::data->currentCamera.lock()
		->getGameObject().lock()->getComponent<Transform>("Transform").lock()
		:
		GameVariables::data->currentLight.lock()->getComponent<Transform>("Transform").lock());

	float speed = 50.0f;

	//move along object along x
	if (InputManager::isKeyHeld(SDLK_a))
	{
		object->translate(glm::vec3(-speed * dt, 0, 0));
	}
	else if (InputManager::isKeyHeld(SDLK_d))
	{
		object->translate(glm::vec3(speed * dt, 0, 0));
	}

	//move object along y
	if (InputManager::isKeyHeld(SDLK_q))
	{
		object->translate(glm::vec3(0, -speed * dt, 0));
	}
	else if (InputManager::isKeyHeld(SDLK_e))
	{
		object->translate(glm::vec3(0, speed * dt, 0));
	}

	//move object along z
	if (InputManager::isKeyHeld(SDLK_w))
	{
		object->translate(glm::vec3(0, 0, -speed * dt));
	}
	if (InputManager::isKeyHeld(SDLK_s))
	{
		object->translate(glm::vec3(0, 0, speed * dt));
	}

	float speedRadians = Utility::convertAngleToRadian(speed * dt);

	//rotate along object along x
	if (InputManager::isKeyHeld(SDLK_UP))
	{
		object->rotate(glm::vec3(speedRadians, 0, 0));
	}
	else if (InputManager::isKeyHeld(SDLK_DOWN))
	{
		object->rotate(glm::vec3(-speedRadians, 0, 0));
	}
	//rotate object along y
	if (InputManager::isKeyHeld(SDLK_LEFT))
	{
		object->rotate(glm::vec3(0, speedRadians, 0));
	}
	if (InputManager::isKeyHeld(SDLK_RIGHT))
	{
		object->rotate(glm::vec3(0, -speedRadians, 0));
	}
	//rotate object along z
	if (InputManager::isKeyHeld(SDLK_k))
	{
		object->rotate(glm::vec3(0, 0, speedRadians));
	}
	if (InputManager::isKeyHeld(SDLK_l))
	{
		object->rotate(glm::vec3(0, 0, -speedRadians));
	}


	if (InputManager::isControllerAxisInUse(0, Controller::Axis2D::LeftStick))
	{
		object->translate(glm::vec3(
			-InputManager::getControllerAxis2D(0, Controller::Axis2D::LeftStick).x * speed * dt,
			0.0f, 
			-InputManager::getControllerAxis2D(0, Controller::Axis2D::LeftStick).y * speed * dt));
	}

}

void Game::collisionResponse()
{
	/*
	if (GameVariables::data->gameObjs["light"]->getComponent<SphereCollider>().lock()->isColliding())
	{
		GameVariables::data->gameObjs["light"]->getComponent<MeshRenderer>().lock()->
			setMaterial(ResourceManager::getMaterial("red", 0, false));
		GameVariables::data->gameObjs["sphere"]->getComponent<MeshRenderer>().lock()->
			setMaterial(ResourceManager::getMaterial("red", 0, false));

		InputManager::playControllerRumble(0, 1, 10);
	}
	else
	{
		GameVariables::data->gameObjs["light"]->getComponent<MeshRenderer>().lock()->
			setMaterial(ResourceManager::getMaterial("blue", 0, false));
		GameVariables::data->gameObjs["sphere"]->getComponent<MeshRenderer>().lock()->
			setMaterial(ResourceManager::getMaterial("blue", 0, false));
	}
	*/
	
}
