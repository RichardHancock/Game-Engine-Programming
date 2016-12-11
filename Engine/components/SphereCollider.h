#pragma once

#include <memory>

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();

	virtual void onUpdate();
	virtual void onAwake();

	bool isColliding();

private:
	bool colliding;

	bool intersectSphere(std::weak_ptr<SphereCollider> other);
	bool intersectAABB(AABB other);
};
