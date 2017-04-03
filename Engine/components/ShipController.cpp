#include "ShipController.h"

#include "RigidBody.h"
#include "Transform.h"
#include "../misc/DeltaTime.h"
#include "../input/InputManager.h"
#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "../ResourceManager.h"
#include "ExpiryTimer.h"
#include "ExplosiveCollision.h"

ShipController::ShipController()
	: reloadTimer(0.0f)
{
	//Preload rocket model
	ResourceManager::getModel("rocket.obj");
}

ShipController::~ShipController()
{
}

void ShipController::onUpdate()
{
	reloadTimer.update();

	movementControls();

	if (InputManager::wasKeyPressed(SDLK_SPACE) && reloadTimer.hasTimerFinished())
	{
		fireRocket();

		reloadTimer.restart(5.0f);
	}
}

void ShipController::movementControls()
{
	std::shared_ptr<GameObject> gameObj = getGameObject().lock();

	std::shared_ptr<RigidBody> objectRB = gameObj->getComponent<RigidBody>().lock();
	std::shared_ptr<Transform> object = gameObj->getComponent<Transform>().lock();

	//Pre-compute the move distance
	const float speed = 5000.0f;
	float speedDT = speed * DeltaTime::getDT();
	glm::vec3 forward = object->getForwardVector();
	glm::vec3 right = object->getRightVector();
	glm::vec3 up = object->getUpVector();

	//move along object along x
	if (InputManager::isKeyHeld(SDLK_a))
	{
		objectRB->applyForce(-speedDT * right);
	}
	else if (InputManager::isKeyHeld(SDLK_d))
	{
		objectRB->applyForce(speedDT * right);
	}

	//move object along y
	if (InputManager::isKeyHeld(SDLK_q))
	{
		objectRB->applyForce(-speedDT * up);
	}
	else if (InputManager::isKeyHeld(SDLK_e))
	{
		objectRB->applyForce(speedDT * up);
	}

	//move object along z
	if (InputManager::isKeyHeld(SDLK_w))
	{
		objectRB->applyForce(-speedDT * forward);
	}
	if (InputManager::isKeyHeld(SDLK_s))
	{
		objectRB->applyForce(speedDT * forward);
	}

	float speedRadians = Utility::convertAngleToRadian(speedDT / 100.0f);

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
		objectRB->applyForce(glm::vec3(
			InputManager::getControllerAxis2D(0, Controller::Axis2D::LeftStick).x * speed * DeltaTime::getDT(),
			0.0f,
			InputManager::getControllerAxis2D(0, Controller::Axis2D::LeftStick).y * speed * DeltaTime::getDT()));
	}
}

void ShipController::fireRocket()
{
	auto shipTransform = getGameObject().lock()->
		getComponent<Transform>("Transform").lock();

	auto rocket = GameObject::create("Rocket", true).lock();
	auto rocketT = rocket->addComponent<Transform>("Transform").lock();

	rocketT->setPosition(shipTransform->getPosition() + (shipTransform->getUpVector() * -15.0f));
	rocketT->setRotation(shipTransform->getRotation());

	rocket->addComponent<MeshComponent>("MeshComponent").lock()
		->setMesh(ResourceManager::getModel("rocket.obj"));

	rocket->addComponent<MeshRenderer>("MeshRenderer").lock()
		->setMaterials(ResourceManager::getMaterials("rocket.obj"));

	rocket->addComponent<CollisionShape>("CollisionShape").lock()
		->generateBoxShape();

	auto rocketRB = rocket->addComponent<RigidBody>("RigidBody").lock();
	rocketRB->init(0.1f, glm::vec3(0.1f));
	rocketRB->applyForce(rocketT->getForwardVector() * -10.0f);

	rocket->addComponent<ExpiryTimer>("ExpiryTimer").lock()->startExpiryTimer(7.0f);

	rocket->addComponent<ExplosiveCollision>("ExplosiveCollision");
}
