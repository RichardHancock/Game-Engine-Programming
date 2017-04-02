#pragma once

#include <memory>
#include <string>

#include "../GameObject.h"

class GameObject; //Forward Specifier

/** @brief	A game object component that can be attached to a gameobject to alter its behaviour and provide functionality. */
class Component
{
	friend class GameObject;

public:

	/** @brief	Destructor. */
	virtual ~Component();

	/**
	 @brief	Gets parent game object.
	
	 @return	The game object.
	 */
	std::weak_ptr<GameObject> getGameObject();

	/**
	 @brief	Gets the type of component.
	
	 @return	The type.
	 */
	std::string getType();

private:

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

	/** @brief	The type of component. */
	std::string type;

	/** @brief	The parent game object reference. */
	std::weak_ptr<GameObject> gameObject;
};