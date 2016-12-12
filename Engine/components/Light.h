#pragma once

#include "Component.h"

#include <glm/glm.hpp>

#include "Transform.h"

class Light : public Component
{
public:
	void init(float newConstant, float newLinear, float newQuadratic
		, glm::vec3 newAmbient, glm::vec3 newDiffuse, glm::vec3 newSpecular);

	virtual ~Light();

	virtual void onAwake();

	glm::vec3 getPosition();

	float getConstant() { return constant; }
	float getLinear() { return linear; }
	float getQuadratic() { return quadratic; }

	glm::vec3 getAmbient() { return ambient; }
	glm::vec3 getDiffuse() { return diffuse; }
	glm::vec3 getSpecular() { return specular; }

protected:

	float constant;
	float linear;
	float quadratic;
	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};