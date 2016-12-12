#pragma once

#include "Component.h"

#include <vector>
#include <memory>

#include "../Material.h"

/** @brief	Component that handles the rendering of meshes and store references to the materials that will be used. */
class MeshRenderer : public Component
{
public:

	/** @brief	Destructor. */
	virtual ~MeshRenderer();

	/**
	 @brief	Gets the first material.
	
	 @return	The material.
	 */
	std::weak_ptr<Material> getMaterial();

	/**
	 @brief	Gets vector of all the materials.
	
	 @return	The materials.
	 */
	std::vector<std::weak_ptr<Material>> getMaterials();

	/**
	 @brief	Sets a material.
	 This always sets the zero'th element of the array

	 @param	material	The material.
	 */
	void setMaterial(std::weak_ptr<Material> material);

	/**
	 @brief	Sets the materials.
	
	 @param	newMaterials	The new materials.
	 */
	void setMaterials(std::vector<std::weak_ptr<Material>> newMaterials);


	/** @brief	Executes the render action. */
	virtual void onRender();

private:

	/** @brief	The referenced materials. */
	std::vector<std::weak_ptr<Material>> materials;
};
