#include "GameObject.h"

#include <assert.h>

#include "components/Transform.h"
#include "misc/GameVariables.h"
#include "misc/Random.h"
#include "misc/Utility.h"

GameObject::GameObject(std::string name)
	: name(name), deleteFlag(false)
{

}

GameObject::~GameObject()
{

}

std::weak_ptr<GameObject> GameObject::create(std::string name, bool makeUnique)
{
	if (makeUnique)
	{
		std::string modifiedName;
		//assign random string of numbers to the name and check if unique
		do
		{
			modifiedName = name;

			modifiedName += Utility::intToString(Random::getInt(1, 9999999));
		} 
		while (GameVariables::data->gameObjs.count(modifiedName) > 0);
		
		name = modifiedName;
	}


	std::shared_ptr<GameObject> gameObj = std::make_shared<GameObject>(name);

	GameVariables::data->gameObjs[name] = gameObj;
	Log::logI("Creating Game Object: " + name);
	return gameObj;
}

std::string GameObject::getName()
{
	return name;
}

void GameObject::setName(std::string newName)
{
	name = newName;
}

bool GameObject::checkDeleteFlag()
{
	return deleteFlag;
}

void GameObject::setDeleteFlag()
{
	deleteFlag = true;
}

void GameObject::onAwake()
{
	
}

void GameObject::onStart()
{
	for (auto component : components)
	{
		component.second->onStart();
	}
}

void GameObject::onUpdate()
{
	for (auto component : components)
	{
		component.second->onUpdate();

		//Check if the item has been flagged for deletion
		if (deleteFlag)
			break;
	}
}

void GameObject::onFixedUpdate()
{
	for (auto component : components)
	{
		component.second->onFixedUpdate();
	}
}

void GameObject::onPreRender()
{
	for (auto component : components)
	{
		component.second->onPreRender();
	}
}

void GameObject::onRender()
{
	for (auto component : components)
	{
		component.second->onRender();
	}
}

void GameObject::onGui()
{
	for (auto component : components)
	{
		component.second->onGui();
	}
}
