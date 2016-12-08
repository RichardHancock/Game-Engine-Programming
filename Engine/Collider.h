#pragma once

#include "Component.h"
#include "AABB.h"
#include "Transform.h"

class Collider : public Component
{
public:
	Collider();

	virtual ~Collider();

	AABB getBounds();

	virtual void onAwake();

	std::shared_ptr<Transform> getTransform();

protected:

	
	bool boundsFetched;

private:
	void updateBounds();
	AABB bounds;
};