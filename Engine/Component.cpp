#include "Component.h"

#include "misc\Utility.h"

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