#pragma once

#include <unordered_map>
#include <memory>

#include "Component.h"

class Component; //Forward Specifier

class GameObject {
public:

	GameObject();

	~GameObject();

	std::weak_ptr<Component> addComponent(std::string title, std::weak_ptr<Component> component);

	template<class T>
	std::weak_ptr<T> getComponent(std::string title);

private:
	std::unordered_map<std::string, std::shared_ptr<Component>> components;
};