#pragma once

#include <unordered_map>
#include <memory>
#include <cassert>
#include <vector>

#include "components/Component.h"
#include "misc/Log.h"
#include "misc/GameVariables.h"

//Ref: Based of Karsten's game object with many changes

class GameVariables;
class Component; //Forward Specifier

/** @brief	A game object that contains components the provide the functionality. */
class GameObject : public std::enable_shared_from_this<GameObject>
{
public:

	/**
	 @brief	Searches for the all objects of templated type.
	
	 @return	The found objects of type.
	 */
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

	/**
	 @brief	Constructor.
	
	 @param	name	The name.
	 */
	GameObject(std::string name);

	/** @brief	Destructor. */
	~GameObject();

	/**
	 @brief	Creates a new game object 
	
	 @param	name	The name.
	
	 @return	reference to new game object
	 */
	static std::weak_ptr<GameObject> create(std::string name);

	/**
	@brief	Adds a component.

	@param	title	The name of the component.

	@return	Reference to the new component;
	*/
	template <class T>
	std::weak_ptr<T> addComponent(std::string title)
	{
		std::shared_ptr<T> c(new T());

		assert(components.count(title) == 0);

		components.insert(std::make_pair(title, c));

		c->gameObject = shared_from_this();
		c->onAwake();

		Log::logD("Adding Component: " + title); 

		return c;
	}

	/**
	@brief	Gets first available component of type.
	@param title The title of the component

	@return	The component.
	*/
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

	/**
	@brief	Gets first available component of type.

	@return	The component.
	*/
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

	/**
	 @brief	Gets the name of the game object.
	
	 @return	The name.
	 */
	std::string getName();

	/**
	 @brief	Sets the name of the game object.
	
	 @param	newName	Name of the game object.
	 */
	void setName(std::string newName);


	/** @brief	Executes the awake action. */
	virtual void onAwake();

	/** @brief	Executes the start action. */
	virtual void onStart();

	/** @brief	Executes the update action. */
	virtual void onUpdate();

	/** @brief	Executes the fixed update action. */
	virtual void onFixedUpdate();

	virtual void onPreRender();

	/** @brief	Executes the render action. */
	virtual void onRender();

	/** @brief	Executes the graphical user interface action. */
	virtual void onGui();

private:
	/** @brief	The components. */
	std::unordered_map<std::string, std::shared_ptr<Component>> components;

	/** @brief	Name of the Game Object. */
	std::string name;

};
