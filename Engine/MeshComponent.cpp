#include "MeshComponent.h"

#include "misc/Log.h"

MeshComponent::~MeshComponent()
{
}

void MeshComponent::setMesh(std::weak_ptr<GameModel> newMesh)
{
	this->mesh = newMesh;
}

std::weak_ptr<GameModel> MeshComponent::getMesh()
{
	if (mesh.expired())
		Log::logW("MeshComponent::getMesh called while no mesh is stored");

	return mesh;
}
