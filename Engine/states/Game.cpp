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
#include "../misc/DeltaTime.h"
#include "../HeightMap.h"
#include "../components/CollisionShape.h"
#include "../components/RigidBody.h"
#include "../misc/debugDrawer/DebugDrawer.h"

#include "../mobileUI/MobileGameUI.h"

Game::Game()
{
	stateName = "Game";

	//MOBILE GAME UI START
	MobileGameUI* mobileUI = new MobileGameUI();
	//MOBILE GAME UI END

	controllingCamera = true;

	//Height Map Test
	heightmap = HeightMap::load("resources/textures/heightmap.bmp", 0.60f, 4.0f);
	auto hmap = GameObject::create("heightmap").lock();
	auto transform58 = hmap->addComponent<Transform>("Transform").lock();
	transform58->setPostion(glm::vec3(30.0f, 0.0f, 0.0f));
	transform58->setScale(glm::vec3(1));

	hmap->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		heightmap);
	//hmap->addComponent<CollisionShape>("CollisionShape").lock()->generateStaticMeshShape();
	//hmap->addComponent<RigidBody>("RigidBody").lock();

	ResourceManager::createMaterial("hmapTex", ResourceManager::getTexture("heightmap.png"),
		"texturedV.glsl", "texturedF.glsl");

	hmap->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterial(
		ResourceManager::getMaterial("hmapTex", 0, false)
	);

	
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

	gameO->addComponent<CollisionShape>("CollisionShape").lock()->generateStaticMeshShape();
	auto rbGameO = gameO->addComponent<RigidBody>("RigidBody").lock();
	//rbGameO->setPosition(glm::vec3(40, 0, 10));
	rbGameO->init(0.0f, glm::vec3(0.0f));


	auto ship = GameObject::create("ship").lock();
	auto transform2 = ship->addComponent<Transform>("Transform").lock();
	transform2->setPostion(glm::vec3(0.0f, 30.0f, 10.0f));
	transform2->setScale(glm::vec3(1));

	ship->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("space_frigate_6.obj"));

	ship->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("space_frigate_6.obj")
	);

	ship->addComponent<CollisionShape>("CollisionShape").lock()->generateStaticMeshShape();
	auto rbShip = ship->addComponent<RigidBody>("RigidBody").lock();
	//rbShip->setPosition(glm::vec3(40, 30, 15));
	rbShip->init(2, glm::vec3(1.0f));

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
	transform4->setPostion(glm::vec3(-20.0f, 50.0f, -350.0f));
	transform4->setRotation(glm::vec3(Utility::HALF_PI, 0.0f, 0.0f));
	transform4->setScale(glm::vec3(100));

	flatPlane->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("flatPlane.obj"));

	flatPlane->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("flatPlane.obj")
	);


	ResourceManager::createMaterial("blue", ResourceManager::getTexture("blue.png"),
		"texturedV.glsl", "texturedF.glsl");
	ResourceManager::createMaterial("red", ResourceManager::getTexture("red.png"),
		"texturedV.glsl", "texturedF.glsl");


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
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.8f, 0.4f, 0.4f),
		glm::vec3(0.5f)
	);

	auto sphere = GameObject::create("sphere").lock();
	auto sphereT = sphere->addComponent<Transform>("Transform").lock();
	sphereT->setPostion(glm::vec3(-40.0f, 0.0f, -5.0f));

	sphere->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("bowl.obj", false));

	sphere->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterial(
		ResourceManager::getMaterial("red" , 0, false)
	);
	sphere->addComponent<SphereCollider>("SphereCollider");

	
	//socket = new Socket("localhost", 8080);

	DebugDrawer::setPointSize(15.0f);


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

	//Statement can be simplified to one line
	if (InputManager::wasKeyReleased(SDLK_ESCAPE))
	{
		return true;
	}

	return false;
}

void Game::update()
{

	InputManager::printDebugInfo();


	if (InputManager::wasKeyPressed(SDLK_1) || InputManager::wasControllerButtonPressed(0,Controller::Button::A))
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
		camera->setPostion(glm::vec3(0, 0, 40));

		GameVariables::data->gameObjs["light"]->getComponent<Transform>().lock()->setPostion(glm::vec3(-50.0f, 10.0f, -5.0f));
		GameVariables::data->gameObjs["sphere"]->getComponent<Transform>().lock()->setPostion(glm::vec3(-40.0f, 0.0f, -5.0f));
	}

	if (InputManager::wasKeyPressed(SDLK_n))
	{
		//socket->sendMsg("Hello");
	}


	//GameVariables::data->gameObjs["fighter"]->getComponent<Transform>("Transform").lock()->rotate(glm::vec3(0.0f, 1.0f * DeltaTime::getDT(), 0.0f));
	GameVariables::data->gameObjs["earth"]->getComponent<Transform>("Transform").lock()->rotate(glm::vec3(0.0f, 0.01f * DeltaTime::getDT(), 0.0f));

	//socket->recvMsg();

	movementControls();

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

void Game::movementControls()
{
	//If camera control mode get pointer to camera transform, else get the light's transform instead
	std::shared_ptr<Transform> object = (controllingCamera ? 
		GameVariables::data->currentCamera.lock()
		->getGameObject().lock()->getComponent<Transform>("Transform").lock()
		:
		//GameVariables::data->currentLight.lock()->getComponent<Transform>("Transform").lock()
		GameVariables::data->gameObjs["fighter"]->getComponent<Transform>().lock());

	//Pre-compute the move distance
	const float speed = 100.0f;
	float speedDT = speed * DeltaTime::getDT();
	glm::vec3 forward = object->getForwardVector();
	glm::vec3 right = object->getRightVector();
	glm::vec3 up = object->getUpVector();


	//move along object along x
	if (InputManager::isKeyHeld(SDLK_a))
	{
		object->translate(-speedDT * right);
	}
	else if (InputManager::isKeyHeld(SDLK_d))
	{
		object->translate(speedDT * right);
	}

	//move object along y
	if (InputManager::isKeyHeld(SDLK_q))
	{
		object->translate(-speedDT * up);
	}
	else if (InputManager::isKeyHeld(SDLK_e))
	{
		object->translate(speedDT * up);
	}

	//move object along z
	if (InputManager::isKeyHeld(SDLK_w))
	{
		object->translate(-speedDT * forward);
	}
	if (InputManager::isKeyHeld(SDLK_s))
	{
		object->translate(speedDT * forward);
	}

	float speedRadians = Utility::convertAngleToRadian(speedDT);

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

	//Controller Movement
	if (InputManager::isControllerAxisInUse(0, Controller::Axis2D::LeftStick))
	{
		object->translate(glm::vec3(
			InputManager::getControllerAxis2D(0, Controller::Axis2D::LeftStick).x * speed * DeltaTime::getDT(),
			0.0f, 
			InputManager::getControllerAxis2D(0, Controller::Axis2D::LeftStick).y * speed * DeltaTime::getDT()));
	}



	//Check if any collisions are active with the light
	if (GameVariables::data->gameObjs["light"]->getComponent<SphereCollider>().lock()->isColliding())
	{
		InputManager::playControllerRumble(0, 1, 100);
		GameVariables::data->gameObjs["light"]->getComponent<MeshRenderer>().lock()->
			setMaterial(ResourceManager::getMaterial("blue", 0, false));
		
		//Set Light emit colour to blue
		std::shared_ptr<Light> light = GameVariables::data->gameObjs["light"]->getComponent<Light>().lock();
		light->setAmbient(glm::vec3(0.0f, 0.0f, 0.9f));
		light->setDiffuse(glm::vec3(0.4f, 0.4f, 0.8f));
	}
	else
	{
		GameVariables::data->gameObjs["light"]->getComponent<MeshRenderer>().lock()->
			setMaterial(ResourceManager::getMaterial("red", 0, false));

		//Set Light emit colour to red
		std::shared_ptr<Light> light = GameVariables::data->gameObjs["light"]->getComponent<Light>().lock();
		light->setAmbient(glm::vec3(0.9f, 0.0f, 0.0f));
		light->setDiffuse(glm::vec3(0.8f, 0.4f, 0.4f));
	}
}
