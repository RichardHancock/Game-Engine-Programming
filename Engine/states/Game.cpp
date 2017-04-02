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
#include "../components/RigidBody.h"
#include "../components/SkyBoxRenderer.h"
#include "../components/ShipController.h"

#include "../mobileUI/QRCode.h"
#include "../misc/Random.h"

Game::Game()
{
	stateName = "Game";

	//MOBILE GAME UI START
	mobileUI = new MobileGameUI();
	//mobileUI->connect("http://gameinput.com");
	qrGenerated = false;

	sentValues.health = shipHealth = 100.0f;
	sentValues.shield = shipShield = 100.0f;
	sentValues.rotation = glm::vec3(0.0f);

	shipHealth = 100.0f;
	shipShield = 100.0f;

	//Preload audio
	ResourceManager::getAudio("hit.wav", false);
	ResourceManager::getAudio("shield.wav", false);
	//MOBILE GAME UI END

	controllingCamera = true;

	//Height Map Test
	heightmap = HeightMap::load(ResourceManager::getResourceDirPath() + "resources/textures/heightmap.bmp", 0.60f, 4.0f);
	auto hmap = GameObject::create("heightmap").lock();
	auto transform58 = hmap->addComponent<Transform>("Transform").lock();
	transform58->setPosition(glm::vec3(30.0f, 0.0f, 0.0f));
	transform58->setScale(glm::vec3(1));

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
	auto camTransform = cameraObj->addComponent<Transform>("Transform").lock();
	camTransform->setPosition(glm::vec3(0, 0, 40));
	camTransform->setEulerRotation(glm::vec3(0.0f, 0, 0));
	camTransform->setScale(glm::vec3(1));
	

	auto cameraComponent = cameraObj->addComponent<Camera>("Camera");
	GameVariables::data->currentCamera = cameraComponent;

	//Game Object
	auto gameO = GameObject::create("fighter").lock();
	auto transform = gameO->addComponent<Transform>("Transform").lock();
	transform->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	transform->setScale(glm::vec3(1));

	gameO->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("dark_fighter_6.obj"));

	gameO->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("dark_fighter_6.obj")
	);

	gameO->addComponent<CollisionShape>("CollisionShape").lock()->generateConvexMeshShape();
	auto rbGameO = gameO->addComponent<RigidBody>("RigidBody").lock();
	rbGameO->init(20.0f, glm::vec3(10.0f));
	rbGameO->setDamping(0.5f, 0.8f);

	gameO->addComponent<ShipController>("ShipController");


	auto ship = GameObject::create("ship").lock();
	auto transform2 = ship->addComponent<Transform>("Transform").lock();
	transform2->setPosition(glm::vec3(70.0f, 60.0f, 40.0f));
	transform2->setScale(glm::vec3(1));

	ship->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("space_frigate_6.obj"));

	ship->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("space_frigate_6.obj")
	);

	ship->addComponent<CollisionShape>("CollisionShape").lock()->generateConvexMeshShape();
	auto rbShip = ship->addComponent<RigidBody>("RigidBody").lock();
	rbShip->init(1.0f, glm::vec3(1.0f));
	rbShip->setDamping(0.1f, 0.1f);

	auto earth = GameObject::create("earth").lock();
	auto transform3 = earth->addComponent<Transform>("Transform").lock();
	transform3->setPosition(glm::vec3(-20.0f, 50.0f, -150.0f));
	transform3->setScale(glm::vec3(50));

	earth->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("Earth.obj"));

	earth->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterials(
		ResourceManager::getMaterials("Earth.obj")
	);

	earth->addComponent<CollisionShape>("CollisionShape").lock()->generateStaticMeshShape();
	auto rbEarth = earth->addComponent<RigidBody>("RigidBody").lock();
	rbEarth->init(0.0f, glm::vec3(0.0f));


	ResourceManager::createMaterial("blue", ResourceManager::getTexture("blue.png"),
		"texturedV.glsl", "texturedF.glsl");
	ResourceManager::createMaterial("red", ResourceManager::getTexture("red.png"),
		"texturedV.glsl", "texturedF.glsl");

	//ALL BELOW ARE LOADED WITH MY OWN OBJ LOADER
	//spheres (Loaded using my own OBJ loader)
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

	auto sphere = GameObject::create("sphere").lock();
	auto sphereT = sphere->addComponent<Transform>("Transform").lock();
	sphereT->setPosition(glm::vec3(-40.0f, 0.0f, -5.0f));

	sphere->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel("bowl.obj", false));

	sphere->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterial(
		ResourceManager::getMaterial("red" , 0, false)
	);
	sphere->addComponent<SphereCollider>("SphereCollider");
	//END OF LOADING USING MY OBJ LOADER
	

	//MODELS LOADED USING PRIMITIVES
	//SkyBox
	/*skyBoxCubeMap = std::make_shared<CubeMap>(
		"resources/textures/cloudtop_rt.png",
		"resources/textures/cloudtop_lf.png",
		"resources/textures/cloudtop_up.png",
		"resources/textures/cloudtop_dn.png",
		"resources/textures/cloudtop_bk.png",
		"resources/textures/cloudtop_ft.png"
		);*/
	skyBoxCubeMap = std::make_shared<CubeMap>(
		"resources/textures/space/right.png",
		"resources/textures/space/left.png",
		"resources/textures/space/top.png",
		"resources/textures/space/bottom.png",
		"resources/textures/space/back.png",
		"resources/textures/space/front.png"
		);

	ResourceManager::createMaterial("BlueSkyBox", skyBoxCubeMap, "skyBoxV.glsl", "skyBoxF.glsl");

	auto skyBox = GameObject::create("SkyBox").lock();

	skyBox->addComponent<Transform>("Transform");
	//Add primitive cube
	skyBox->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
		ResourceManager::getModel(ResourceManager::ModelPrimitives::Cube)
	);

	skyBox->addComponent<SkyBoxRenderer>("Sky").lock()->setMaterial(
		ResourceManager::getMaterial("BlueSkyBox", 0, false)
	);
	
	//socket = new Socket("localhost", 8080);

	DebugDrawer::setPointSize(15.0f);


	//Loading finished trigger onAwake
	for (auto gameObj : GameVariables::data->gameObjs)
	{
		gameObj.second->onAwake();
	}
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

	mobileUIEventQueue();

	return false;
}

void Game::update()
{

	InputManager::printDebugInfo();


	if (InputManager::wasKeyPressed(SDLK_1) || InputManager::wasControllerButtonPressed(0,Controller::Button::A))
	{
		ResourceManager::getAudio("Item Place.wav", false).lock()->play(0,0);
	}
	
	//reset scene
	if (InputManager::wasKeyReleased(SDLK_SPACE) || InputManager::wasControllerButtonPressed(0, Controller::Button::B))
	{
		//std::shared_ptr<Transform> camera = GameVariables::data->currentCamera.lock()->getGameObject().lock()
			//->getComponent<Transform>("Transform").lock();

		//camera->setEulerRotation(glm::vec3(0.0f));
		//camera->setPosition(glm::vec3(0, 0, 40));

		//GameVariables::data->gameObjs["light"]->getComponent<Transform>().lock()->setPosition(glm::vec3(-50.0f, 10.0f, -5.0f));
		//GameVariables::data->gameObjs["sphere"]->getComponent<Transform>().lock()->setPosition(glm::vec3(-40.0f, 0.0f, -5.0f));
	}

	if (InputManager::wasKeyPressed(SDLK_n))
	{
		//socket->sendMsg("Hello");
	}


	//GameVariables::data->gameObjs["fighter"]->getComponent<Transform>("Transform").lock()->rotate(glm::vec3(0.0f, 1.0f * DeltaTime::getDT(), 0.0f));
	GameVariables::data->gameObjs["earth"]->getComponent<Transform>("Transform").lock()->rotate(glm::vec3(0.0f, 0.01f * DeltaTime::getDT(), 0.0f));

	//socket->recvMsg();

	


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

	movementControls();


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

	//mobileUIUpdate();
}

void Game::render()
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

void Game::movementControls()
{
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

void Game::mobileUIUpdate()
{
	if (!qrGenerated &&
		mobileUI->getSessionID() != "" &&
		mobileUI->isConnected())
	{
		std::string qrText = "http://gameinput.com/?id=" + mobileUI->getSessionID();

		QRCode qr(qrText, qrcodegen::QrCode::Ecc::HIGH);

		SDL_Surface* qrSurface = qr.convertToSurface(10);
		qrTexture = std::make_shared<Texture>(qrSurface); //NEED TO DELETE AT SOME POINT
		//SDL_SaveBMP(qrSurface, "testing.bmp");
		auto qrObject = GameObject::create("QRCode").lock();
		
		auto qrTransform = qrObject->addComponent<Transform>("Transform").lock();
		qrTransform->setPosition(glm::vec3(-5.0f, 0.0f, 20.0f));
		qrTransform->setEulerRotation(glm::vec3(Utility::HALF_PI, 0.0f, 0.0f));
		qrTransform->setScale(glm::vec3(2));

		qrObject->addComponent<MeshComponent>("MeshComponent").lock()->setMesh(
			ResourceManager::getModel("flatPlaneRaw.obj", false));

		ResourceManager::createMaterial("QR", qrTexture,
			"texturedV.glsl", "texturedF.glsl");

		qrObject->addComponent<MeshRenderer>("MeshRenderer").lock()->setMaterial(
			ResourceManager::getMaterial("QR", 0, false)
		);

		qrGenerated = true;
	}
	
	//Health Recharge
	if (InputManager::wasKeyReleased(SDLK_r))
	{
		shipShield = 100.00f;
		shipHealth = 100.00f;
	}



	//Update Values and send
	if (qrGenerated && mobileUI->isConnected())
	{
		auto ship = GameVariables::data->gameObjs["fighter"]->getComponent<Transform>().lock();

		glm::vec3 rotation = ship->getEulerRotation();

		if (rotation != sentValues.rotation)
		{
			//Send updated rotation data to connected clients. (Reduced to 2 decimal places as we don't need that much precision and saves bandwidth)
			mobileUI->send("shipYaw", Utility::floatToString(
				Utility::convertRadiansToAngle(rotation.y), 2)
			);
			mobileUI->send("shipPitch", Utility::floatToString(
				Utility::convertRadiansToAngle(rotation.x), 2)
			);
			mobileUI->send("shipRoll", Utility::floatToString(
				Utility::convertRadiansToAngle(rotation.z), 2)
			);

			sentValues.rotation = rotation;
		}

		
		//Check Other Vars
		if (shipShield != sentValues.shield)
		{
			mobileUI->send("shipShield", Utility::floatToString(shipShield, 2));
			sentValues.shield = shipShield;
		}

		if (shipHealth != sentValues.health)
		{
			mobileUI->send("shipHealth", Utility::floatToString(shipHealth, 2));
			sentValues.health = shipHealth;
		}
	}
}

void Game::mobileUIEventQueue()
{
	//While there are events to process	
	while (!mobileUI->isQueueEmpty())
	{
		std::string eventMsg = mobileUI->pollQueue();
		assert(eventMsg != "");

		//NEED TO REWORK THIS TO A ENUM 
		if (eventMsg == "qrToggle")
		{
			toggleQRVisiblity();
			continue;
		}

		if (eventMsg == "damageShield")
		{
			float tempShield = shipShield - Random::getFloat(5.0f, 25.0f);
			shipShield = (tempShield < 0.00f ? 0.00f : tempShield);

			ResourceManager::getAudio("shield.wav", false).lock()->play(0, 0);
			continue;
		}

		if (eventMsg == "damageHP")
		{
			float tempHealth = shipHealth - Random::getFloat(5.0f, 25.0f);
			shipHealth = (tempHealth < 0.00f ? 0.00f : tempHealth);

			ResourceManager::getAudio("hit.wav", false).lock()->play(0, 0);
			continue;
		}
	}

	return;
}

void Game::toggleQRVisiblity()
{
	try 
	{
		auto qrRef = GameVariables::data->gameObjs.at("QRCode");

		if (qrRef.get() == nullptr)
			return;

		if (qrRef->getComponent<MeshRenderer>().expired())
			return;

		qrRef->getComponent<MeshRenderer>().lock()->toggleVisibility();
	}
	catch (std::out_of_range)
	{
		return;
	}
}
