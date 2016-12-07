#include "Mesh.h"

#include <cassert>

#include "misc/Log.h"
#include "misc/Vertex.h"

Mesh::Mesh()
{
	numIndicies = 0;
	baseVertex = 0;
	baseIndex = 0;
	materialIndex = 0;
}

Mesh::Mesh(
	unsigned int numIndicies, 
	unsigned int baseVertex, 
	unsigned int baseIndex, 
	unsigned int materialIndex
) : numIndicies(numIndicies),
	baseVertex(baseVertex),
	baseIndex(baseIndex),
	materialIndex(materialIndex)
{
}
