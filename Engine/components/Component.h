#pragma once

#include <memory>
#include <string>

#include "../GameObject.h"

class GameObject; //Forward Specifier

class Component
{
	friend class GameObject;

public:

	virtual ~Component();

	std::weak_ptr<GameObject> getGameObject();

	std::string getType();

private:

	virtual void onAwake();

	virtual void onStart();

	virtual void onUpdate();

	virtual void onFixedUpdate();

	virtual void onRender();

	virtual void onGui();

	std::string type;

	std::weak_ptr<GameObject> gameObject;
};