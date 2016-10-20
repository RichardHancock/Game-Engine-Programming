#include "GameObject.h"

#include <assert.h>

#include "misc/Log.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

std::weak_ptr<Component> GameObject::addComponent(std::string title, std::weak_ptr<Component> component)
{
	std::shared_ptr<Component> c = component.lock();
	assert(!c);

	assert(components.count(title) == 0);

	components.insert(std::make_pair(title, c));
	return c;
}

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
