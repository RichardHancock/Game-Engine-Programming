#include "Graphics.h"

void Graphics::renderMesh(std::weak_ptr<GameModel> mesh, std::weak_ptr<Material> material)
{
	std::shared_ptr<GameModel> meshPtr = mesh.lock();

	glBindVertexArray(meshPtr->getVAO());

	if (meshPtr->hasIndexBuffer())
	{
		glDrawElements(GL_TRIANGLES, meshPtr->getNumIndices(), GL_UNSIGNED_INT, (void*)0);
	} 
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, meshPtr->getNumVertices());
	}
}


void Graphics::renderMesh(std::weak_ptr<GameModel> mesh, unsigned int index, std::weak_ptr<Material> material)
{
	std::shared_ptr<GameModel> meshPtr = mesh.lock();

	glBindVertexArray(meshPtr->getVAO());

	Mesh subMesh = meshPtr->getSubmesh(index);

	if (meshPtr->hasIndexBuffer())
	{
		glDrawElementsBaseVertex(
			GL_TRIANGLES,
			subMesh.numIndicies,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * subMesh.baseIndex),
			subMesh.baseVertex
		);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, meshPtr->getNumVertices());
	}
}
