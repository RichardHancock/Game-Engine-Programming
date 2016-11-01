#pragma once

#include <unordered_map>
#include <memory>

#include "Component.h"


class Component; //Forward Specifier

class GameObject : std::enable_shared_from_this<GameObject>
{
public:

	std::weak_ptr<GameObject> self;

	GameObject(std::string name);

	~GameObject();
	
	template <class T>
	std::weak_ptr<T> addComponent(std::string title)
	{
		std::shared_ptr<T> c(new T());

		assert(components.count(title) == 0);

		components.insert(std::make_pair(title, c));
		
		if (!self.expired()) {
			std::shared_ptr<GameObject> me = shared_from_this();
		}
		

		c->gameObject = self.lock();
		c->onAwake();

		Log::logI("Addding Component: " + title);

		return c;
	}

	template<class T>
	std::weak_ptr<T> getComponent(std::string title)
	{
		//Reference Karsten (Mutiny)
		for (auto component : components)
		{
			std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(component.second);
			
			if (c.get() != nullptr)
			{
				return c;
			}
		}

		return std::weak_ptr<T>();
	}


	std::string getName();

	void setName(std::string newName);

private:
	std::unordered_map<std::string, std::shared_ptr<Component>> components;

	std::string name;
	
};