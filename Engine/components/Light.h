#pragma once

#include "Component.h"

#include <glm/glm.hpp>

#include "Transform.h"

/** @brief	Represents a point light with all properties configurable . */
class Light : public Component
{
public:

	/**
	 @brief	S.
	
	 @param	newConstant 	constant.
	 @param	newLinear   	linear.
	 @param	newQuadratic	quadratic.
	 @param	newAmbient  	ambient.
	 @param	newDiffuse  	diffuse.
	 @param	newSpecular 	specular.
	 */
	void init(float newConstant, float newLinear, float newQuadratic
		, glm::vec3 newAmbient, glm::vec3 newDiffuse, glm::vec3 newSpecular);

	/** @brief	Destructor. */
	virtual ~Light();

	/** @brief	Executes the awake action. */
	virtual void onAwake();

	/**
	 @brief	Gets the position of the light.
	 Shortcut to access the transform component of the game object
	 @return	The position.
	 */
	glm::vec3 getPosition();

	/**
	 @brief	Gets the constant component of the point light.
	
	 @return	The constant.
	 */
	float getConstant() { return constant; }

	/**
	@brief	Gets the linear component of the point light.

	@return	The linear.
	*/
	float getLinear() { return linear; }
	
	/**
	@brief	Gets the quadratic component of the point light.

	@return	The quadratic.
	*/
	float getQuadratic() { return quadratic; }

	/**
	@brief	Gets the ambiend component of the point light.

	@return	The ambient.
	*/
	glm::vec3 getAmbient() { return ambient; }

	/**
	@brief	Gets the diffuse component of the point light.

	@return	The diffuse.	
	*/
	glm::vec3 getDiffuse() { return diffuse; }
	
	/**
	@brief	Gets the specular component of the point light.

	@return	The specular.
	*/
	glm::vec3 getSpecular() { return specular; }


	/**
	 @brief	Sets the ambient component.
	
	 @param	newAmbient	The new ambient.
	 */
	void setAmbient(glm::vec3 newAmbient) { ambient = newAmbient; }

	/**
	 @brief	Sets the diffuse component.
	
	 @param	newDiffuse	The new diffuse.
	 */
	void setDiffuse(glm::vec3 newDiffuse) { diffuse = newDiffuse; }

	/**
	 @brief	Sets the specular component.
	
	 @param	newSpecular	The new specular.
	 */
	void setSpecular(glm::vec3 newSpecular) { specular = newSpecular; }
protected:

	/** @brief	The constant. */
	float constant;
	/** @brief	The linear. */
	float linear;
	/** @brief	The quadratic. */
	float quadratic;
	
	/** @brief	The ambient. */
	glm::vec3 ambient;
	/** @brief	The diffuse. */
	glm::vec3 diffuse;
	/** @brief	The specular. */
	glm::vec3 specular;
};