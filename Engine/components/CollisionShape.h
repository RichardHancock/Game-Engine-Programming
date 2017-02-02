#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>

#include "Component.h"

class CollisionShape : public Component
{
public:

	bool generateStaticMeshShape();

	btCollisionShape* getShape();
private:

	btCollisionShape* shape;
};