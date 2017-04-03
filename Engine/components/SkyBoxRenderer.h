#pragma once

#include "Component.h"

#include "../Material.h"

/** @brief	A sky box renderer. */
class SkyBoxRenderer : public Component
{
public:
	/** @brief	Destructor. */
	virtual ~SkyBoxRenderer();

	/**
	 @brief	Sets a material to be applied to the skybox.
	
	 @param	material	The material.
	 */
	void setMaterial(std::weak_ptr<Material> material);

	/** @brief	Executes the pre render action. */
	virtual void onPreRender();

protected:
	
	/** @brief	The material reference. */
	std::weak_ptr<Material> materialRef;

};