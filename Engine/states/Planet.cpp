#include "Planet.h"

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
#include "../components/RigidBody.h"
#include "../components/SkyBoxRenderer.h"
#include "../components/ShipController.h"
#include "../components/Camera.h"

PlanetState::PlanetState()
{
	//Height Map Test
	heightmap = HeightMap::load(ResourceManager::getResourceDirPath() + "resources/textures/heightmap.bmp", 1.50f, 75.0f);
	auto hmap = GameObject::create("heightmap").lock();
	auto transform58 = hmap->addComponent<Transform>("Transform").lock();
	transform58->setPosition(glm::vec3(-256.0f * 30.0f, 0.0f, -256.0f * 30.0f));

	hmap->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		heightmap);
	hmap->addComponent<CollisionShape>("CollisionShape").lock()->generateStaticMeshShape();
	hmap->addComponent<RigidBody>("RigidBody").lock()->init(0.0f, glm::vec3(0.0f));

	ResourceManager::createMaterial("hmapTex", ResourceManager::getTexture("heightmap.png"),
		"texturedV.glsl", "texturedF.glsl");

	hmap->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterial(
		ResourceManager::getMaterial("hmapTex", 0, false)
	);

	//Camera
	auto cameraObj = GameObject::create("Camera").lock();
	auto camTransform = cameraObj->addComponent<Transform>("Transform");

	auto cameraComponent = cameraObj->addComponent<Camera>("Camera");
	GameVariables::data->currentCamera = cameraComponent;


	//Game Object
	auto gameO = GameObject::create("fighter").lock();
	auto transform = gameO->addComponent<Transform>("Transform").lock();
	transform->setPosition(glm::vec3(0.0f, 150.0f, 5.0f));
	transform->setScale(glm::vec3(1));

	gameO->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("dark_fighter_6.obj"));

	gameO->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("dark_fighter_6.obj")
	);

	gameO->addComponent<CollisionShape>("CollisionShape").lock()->generateConvexMeshShape();
	auto rbGameO = gameO->addComponent<RigidBody>("RigidBody").lock();
	rbGameO->init(20.0f, glm::vec3(1.0f));
	rbGameO->setDamping(0.5f, 0.8f);

	gameO->addComponent<ShipController>("ShipController");



	auto light = GameObject::create("light").lock();
	auto lightT = light->addComponent<Transform>("Transform").lock();
	lightT->setPosition(glm::vec3(-50.0f, 10.0f, -5.0f));

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



	//MODELS LOADED USING PRIMITIVES
	//SkyBox
	skyBoxCubeMap = std::make_shared<CubeMap>(
	"resources/textures/cloudtop_rt.png",
	"resources/textures/cloudtop_lf.png",
	"resources/textures/cloudtop_up.png",
	"resources/textures/cloudtop_dn.png",
	"resources/textures/cloudtop_bk.png",
	"resources/textures/cloudtop_ft.png"
	);

	ResourceManager::createMaterial("BlueSkyBox", skyBoxCubeMap, "skyBoxV.glsl", "skyBoxF.glsl");

	auto skyBox = GameObject::create("BlueSkyBox").lock();

	skyBox->addComponent<Transform>("Transform");
	//Add primitive cube
	skyBox->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel(ResourceManager::ModelPrimitives::Cube)
	);

	skyBox->addComponent<SkyBoxRenderer>("Sky").lock()->setMaterial(
		ResourceManager::getMaterial("BlueSkyBox", 0, false)
	);

	//Loading finished trigger onAwake
	for (auto gameObj : GameVariables::data->gameObjs)
	{
		gameObj.second->onAwake();
	}
}

PlanetState::~PlanetState()
{
	
}

bool PlanetState::eventHandler()
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

void PlanetState::update()
{
	InputManager::printDebugInfo();


	//Calculate Camera Pos
	std::shared_ptr<Transform> camera = GameVariables::data->currentCamera.lock()
		->getGameObject().lock()->getComponent<Transform>("Transform").lock();

	auto ship = GameVariables::data->gameObjs["fighter"]->getComponent<Transform>().lock();

	glm::vec3 shipPos = ship->getPosition();
	glm::vec3 shipOffset(0.0f);
	shipOffset = (ship->getForwardVector() * 45.0f);
	shipOffset = shipOffset + (ship->getUpVector() * 10.0f);

	shipPos = shipPos + shipOffset;

	camera->setPosition(shipPos);
	camera->lookAt(ship->getPosition());

	//Game Object Update (should later be moved to a central area)
	std::vector<std::string> gameObjsToDelete;

	for (auto object : GameVariables::data->gameObjs)
	{
		object.second->onUpdate();

		//Check if objects delete flag has been set
		if (object.second->checkDeleteFlag())
			gameObjsToDelete.push_back(object.first);
	}

	for (std::string objectName : gameObjsToDelete)
	{
		GameVariables::data->gameObjs.erase(objectName);
	}
}

void PlanetState::render()
{
	//preRender (Currently only used for the skybox)
	for (auto object : GameVariables::data->gameObjs)
	{
		object.second->onPreRender();
	}


	//onRender
	for (auto object : GameVariables::data->gameObjs)
	{
		object.second->onRender();
	}
}
