#pragma once

#include <vector>
#include <memory>

#include "Component.h"

class Component; //Forward Specifier

class GameObject {
public:

	template<class T>
	T addComponent(std::weak_ptr<T> component);


private:

	std::vector<std::shared_ptr<Component>> components;
};