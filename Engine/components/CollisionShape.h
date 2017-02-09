#pragma once

#include <memory>

//This is done because bullet is full of warnings and Visual Studio pollutes my warnings with them. This only disables warning in bullet, not my code.
#pragma warning(push, 0) 
#include <btBulletDynamicsCommon.h>
#pragma warning(pop)

#include "Component.h"

class CollisionShape : public Component
{
public:

	bool generateStaticMeshShape();

	btCollisionShape* getShape();
private:

	btCollisionShape* shape;
};