#pragma once

#include <unordered_map>
#include <memory>
#include <cassert>
#include <vector>

#include "components/Component.h"
#include "misc/Log.h"
#include "misc/GameVariables.h"

class GameVariables;
class Component; //Forward Specifier

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:

	template<class T>
	static std::vector<std::weak_ptr<T>> findObjectsOfType()
	{
		std::unordered_map<std::string, std::shared_ptr<GameObject>> gameObjs = GameVariables::data->gameObjs;
		std::vector<std::weak_ptr<T>> results;

		for (auto gameObj : gameObjs)
		{
			std::weak_ptr<T> component = std::dynamic_pointer_cast<T>(gameObj.second->getComponent<T>().lock());

			if (!component.expired())
			{
				results.push_back(component);
			}
		}

		return results;
	}


	GameObject(std::string name);

	~GameObject();

	static std::weak_ptr<GameObject> create(std::string name);

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
		///TODO Add title use here for advanced searches
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

	template<class T>
	std::weak_ptr<T> getComponent()
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
