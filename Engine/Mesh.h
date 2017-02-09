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
	
	 @param	numIndices  	Number of indices.
	 @param	baseVertex   	The starting vertex.
	 @param	baseIndex	 	The starting index.
	 @param	materialIndex	Zero-based index of the material.
	 */
	Mesh(
		unsigned int numIndices,
		unsigned int baseVertex,
		unsigned int baseIndex,
		unsigned int materialIndex
	);

	/** @brief	Number of indices. */
	unsigned int numIndices;
	/** @brief	The starting vertex. */
	unsigned int baseVertex;
	/** @brief	The starting index */
	unsigned int baseIndex;
	/** @brief	Zero-based index of the material. */
	unsigned int materialIndex;
};