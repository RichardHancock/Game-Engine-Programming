#pragma once

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <GL/glew.h>

/** @brief	A sub mesh. */
struct Mesh
{
	/** @brief	Default constructor. */
	Mesh();

	/**
	 @brief	Constructor.
	
	 @param	numIndicies  	Number of indicies.
	 @param	baseVertex   	The starting vertex.
	 @param	baseIndex	 	The starting index.
	 @param	materialIndex	Zero-based index of the material.
	 */
	Mesh(
		unsigned int numIndicies,
		unsigned int baseVertex,
		unsigned int baseIndex,
		unsigned int materialIndex
	);

	/** @brief	Number of indicies. */
	unsigned int numIndicies;
	/** @brief	The starting vertex. */
	unsigned int baseVertex;
	/** @brief	The starting index */
	unsigned int baseIndex;
	/** @brief	Zero-based index of the material. */
	unsigned int materialIndex;
};