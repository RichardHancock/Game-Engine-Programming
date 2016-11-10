#include "GameObject.h"

#include <assert.h>

#include "Transform.h"

GameObject::GameObject(std::string name)
	: name(name)
{

}

GameObject::~GameObject()
{

}

/*
template <class T>
std::weak_ptr<T> GameObject::addComponent(std::string title)
{
	std::shared_ptr<T> c(new T());

	assert(components.count(title) == 0);

	components.insert(std::make_pair(title, component));
	c->gameObject = std::weak_ptr<GameObject>(self);
	c->onAwake();

	return component;
}*/

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
	for (auto component : components)
	{
		component.second->onAwake();
	}
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

/*
template<class T>
std::weak_ptr<T> GameObject::getComponent(std::string title)
{
	//Reference Karsten (Mutiny)
	for (auto component : components)
	{
		std::weak_ptr<T> c = dynamic_cast<T*>(component.second.get());

		if (c.valid())
		{
			return c;
		}
	}

	return std::weak_ptr<T>();
}
*/
