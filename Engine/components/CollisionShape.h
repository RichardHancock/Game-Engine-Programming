#pragma once

#include <memory>

#include "../internal/dependencies/btBulletDynamicsCommon.h"

#include "Component.h"

class CollisionShape : public Component
{
public:

	~CollisionShape();

	bool generateStaticMeshShape();

	bool generateConvexMeshShape();

	bool generateBoxShape();

	btCollisionShape* getShape();
private:

	btCollisionShape* shape;
};