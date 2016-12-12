#include "Light.h"

void Light::init(float newConstant, float newLinear, float newQuadratic,
	glm::vec3 newAmbient, glm::vec3 newDiffuse, glm::vec3 newSpecular)
{
	constant = newConstant;
	linear = newLinear;
	quadratic = newQuadratic;
	ambient = newAmbient;
	diffuse = newDiffuse;
	specular = newSpecular;
}

Light::~Light()
{
}

void Light::onAwake()
{
}

glm::vec3 Light::getPosition()
{
	glm::mat4 mat = getGameObject().lock()->getComponent<Transform>().lock()->getTransformMat();

	return glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
}
