#pragma once

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <GL/glew.h>

struct Mesh
{
	Mesh();

	Mesh(
		unsigned int numIndicies,
		unsigned int baseVertex,
		unsigned int baseIndex,
		unsigned int materialIndex
	);

	unsigned int numIndicies;
	unsigned int baseVertex;
	unsigned int baseIndex;
	unsigned int materialIndex;
};