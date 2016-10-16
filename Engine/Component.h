#pragma once

#include <memory>
#include <string>

#include "GameObject.h"

class GameObject; //Forward Specifier

class Component
{
public:

	virtual ~Component();

	std::weak_ptr<GameObject> getGameObject();

	std::string getType();

private:

	virtual void onAwake() = 0;

	virtual void onStart() = 0;

	virtual void onUpdate() = 0;

	virtual void onFixedUpdate() = 0;

	virtual void onRender() = 0;

	virtual void onGui() = 0;

	std::string type;

	std::weak_ptr<GameObject> gameObject;
};