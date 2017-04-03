#include "Component.h"

#include <assert.h>

Component::~Component() 
{

}

std::weak_ptr<GameObject> Component::getGameObject()
{
	return gameObject;
}

std::string Component::getType()
{
#ifdef _DEBUG
	assert(!type.empty());
#endif

	return type;
}

void Component::onAwake()
{
}

void Component::onStart()
{
}

void Component::onUpdate()
{
}

void Component::onFixedUpdate()
{
}

void Component::onPreRender()
{
}

void Component::onCollision(std::string objAName, std::string objBName, glm::vec3 contactPoint)
{
}

void Component::onRender()
{
}

void Component::onGui()
{
}
