#pragma once

#include <unordered_map>
#include <memory>
#include <cassert>

#include "Component.h"
#include "misc/Log.h"


class Component; //Forward Specifier

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:

	GameObject(std::string name);

	~GameObject();

	template <class T>
	std::weak_ptr<T> addComponent(std::string title)
	{
		std::shared_ptr<T> c(new T());

		assert(components.count(title) == 0);

		components.insert(std::make_pair(title, c));

		c->gameObject = shared_from_this();
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


	virtual void onAwake();

	virtual void onStart();

	virtual void onUpdate();

	virtual void onFixedUpdate();

	virtual void onRender();

	virtual void onGui();

private:
	std::unordered_map<std::string, std::shared_ptr<Component>> components;

	std::string name;

};
