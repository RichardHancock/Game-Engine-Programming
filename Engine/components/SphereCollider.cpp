#include "SphereCollider.h"

#include <vector>
#include <memory>

SphereCollider::SphereCollider()
{
	colliding = false;
}

void SphereCollider::onUpdate()
{
	colliding = false;

	std::vector<std::weak_ptr<Collider>> colObjects = GameObject::findObjectsOfType<Collider>();

	for (auto collider : colObjects)
	{
		//Attempt Sphere Collision test
		std::weak_ptr<SphereCollider> otherSphere = collider.lock()->getGameObject().lock()
			->getComponent<SphereCollider>();

		if (!otherSphere.expired())
		{
			colliding = intersectSphere(otherSphere);
		}
	}

	if (colliding)
	{
		Log::logI("Collision");
	}
}

void SphereCollider::onAwake()
{
	Collider::onAwake();
}

bool SphereCollider::isColliding()
{
	return colliding;
}

bool SphereCollider::intersectSphere(std::weak_ptr<SphereCollider> other)
{
	std::shared_ptr<SphereCollider> otherPtr = other.lock();

	//Distance between spheres
	float distance = glm::length(
		getTransform()->getPosition() - otherPtr->getTransform()->getPosition()
	);

	//Test against self (Replace with better comparison)
	if (distance == 0)
		return false;

	float totalRadius = getBounds().getRadius() + otherPtr->getBounds().getRadius();

	return distance <= totalRadius;
}
