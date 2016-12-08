#include "Collider.h"

#include "misc/Log.h"
#include "Transform.h"
#include "MeshComponent.h"

Collider::Collider()
	: boundsFetched(false)
{
}

Collider::~Collider()
{
}

AABB Collider::getBounds()
{
	if (!boundsFetched)
	{
		updateBounds();
	}

	return bounds;
}

void Collider::onAwake()
{
	updateBounds();
}

std::shared_ptr<Transform> Collider::getTransform()
{
	std::weak_ptr<Transform> transform = getGameObject().lock()->getComponent<Transform>("Transform");

	if (transform.expired())
	{
		Log::logW("GameObject does not have a transform component in Collider");
		return std::shared_ptr<Transform>();
	}

	return transform.lock();
}

void Collider::updateBounds()
{
	std::weak_ptr<MeshComponent> meshCPtr = getGameObject().lock()->getComponent<MeshComponent>("MeshComponent");
	
	if (meshCPtr.expired())
	{
		Log::logW("GameObject does not have a  MeshComponent in Collider");
		return;
	}

	auto mesh = meshCPtr.lock()->getMesh();
	if (mesh.expired())
	{
		Log::logW("MeshComponent does not have a  Mesh in Collider");
		return;
	}

	bounds = mesh.lock()->getAABB();
	boundsFetched = true;
}
