#include "GameObject.h"

#include <assert.h>

#include "components/Transform.h"
#include "misc/GameVariables.h"

GameObject::GameObject(std::string name)
	: name(name)
{

}

GameObject::~GameObject()
{

}

std::weak_ptr<GameObject> GameObject::create(std::string name)
{
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
	}
}

void GameObject::onFixedUpdate()
{
	for (auto component : components)
	{
		component.second->onFixedUpdate();
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
