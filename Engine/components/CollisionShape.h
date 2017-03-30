#pragma once

#include <memory>

#include "../internal/dependencies/btBulletDynamicsCommon.h"

#include "Component.h"

class CollisionShape : public Component
{
public:

	bool generateStaticMeshShape();

	bool generateConvexMeshShape();

	btCollisionShape* getShape();
private:

	btCollisionShape* shape;
};