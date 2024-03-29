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

	if ((InputManager::wasKeyPressed(SDLK_SPACE) || InputManager::wasControllerButtonPressed(0, Controller::Button::A)) && reloadTimer.hasTimerFinished())
	{
		fireRocket();

		reloadTimer.restart(2.0f);
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
	if (InputManager::isKeyHeld(SDLK_q) || InputManager::isControllerButtonHeld(0, Controller::Button::Y))
	{
		objectRB->applyForce(speedDT * up);
	}
	else if (InputManager::isKeyHeld(SDLK_e) || InputManager::isControllerButtonHeld(0, Controller::Button::X))
	{
		objectRB->applyForce(-speedDT * up);
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
		glm::vec3 rightMovement = InputManager::getControllerAxis2D(0, Controller::Axis2D::LeftStick).x * speedDT * right;
		glm::vec3 forwardMovement = InputManager::getControllerAxis2D(0, Controller::Axis2D::LeftStick).y * speedDT * forward;

		objectRB->applyForce(glm::vec3(rightMovement + forwardMovement));
	}

	if (InputManager::isControllerAxisInUse(0, Controller::Axis2D::RightStick))
	{
		object->rotate(glm::vec3(
			InputManager::getControllerAxis2D(0, Controller::Axis2D::RightStick).y * speedRadians * 50.0f * DeltaTime::getDT(),
			0.0f,
			-InputManager::getControllerAxis2D(0, Controller::Axis2D::RightStick).x * speedRadians * 50.0f * DeltaTime::getDT()));
	}

	if (InputManager::isControllerButtonHeld(0, Controller::Button::LEFTSHOULDER))
	{
		object->rotate(glm::vec3(0.0f, speedRadians, 0.0f));
	}
	else if (InputManager::isControllerButtonHeld(0, Controller::Button::RIGHTSHOULDER))
	{
		object->rotate(glm::vec3(0.0f, -speedRadians, 0.0f));
	}
}

void ShipController::fireRocket()
{
	auto shipTransform = getGameObject().lock()->
		getComponent<Transform>("Transform").lock();

	auto rocket = GameObject::create("Rocket", true).lock();
	auto rocketT = rocket->addComponent<Transform>("Transform").lock();

	rocketT->setPosition(shipTransform->getPosition() + (shipTransform->getUpVector() * -15.0f));
	
	btQuaternion quat = shipTransform->getRotation();
	rocketT->setRotation(quat);

	rocket->addComponent<MeshComponent>("MeshComponent").lock()
		->setMesh(ResourceManager::getModel("rocket.obj"));

	rocket->addComponent<MeshRenderer>("MeshRenderer").lock()
		->setMaterials(ResourceManager::getMaterials("rocket.obj"));

	rocket->addComponent<CollisionShape>("CollisionShape").lock()
		->generateBoxShape();

	auto rocketRB = rocket->addComponent<RigidBody>("RigidBody").lock();
	rocketRB->init(1.0f, glm::vec3(1.0f));
	rocketRB->applyForce(rocketT->getForwardVector() * DeltaTime::getDT() * -9000.0f);
	
	rocket->addComponent<ExpiryTimer>("ExpiryTimer").lock()->startExpiryTimer(10.0f);

	rocket->addComponent<ExplosiveCollision>("ExplosiveCollision");
}
