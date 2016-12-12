#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "GameModel.h"
#include "Material.h"

/** @brief	Class that handle the final stage of rendering. */
class Graphics
{
public:

	/**
	 @brief	Renders the mesh.
	
	 @param	mesh		The mesh.
	 @param	material	The material.
	 */
	static void renderMesh(std::weak_ptr<GameModel> mesh, std::weak_ptr<Material> material);

	/**
	 @brief	Renders the mesh.
	
	 @param	mesh		The mesh.
	 @param	index   	Zero-based index of the current submesh.
	 @param	material	The material.
	 */
	static void renderMesh(std::weak_ptr<GameModel> mesh, unsigned int index, std::weak_ptr<Material> material);
};
