#pragma once

#include "Component.h"

class ExplosiveCollision : public Component
{
public:

	virtual ~ExplosiveCollision();

	virtual void onCollision(std::string objAName, std::string objBName, glm::vec3 contactPoint);

protected:
};