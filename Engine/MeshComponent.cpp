#include "MeshComponent.h"

#include "misc/Log.h"

MeshComponent::~MeshComponent()
{
}

void MeshComponent::setMesh(std::weak_ptr<GameModel> mesh)
{
	this->mesh = mesh;
}

std::weak_ptr<GameModel> MeshComponent::getMesh()
{
	if (mesh.expired())
		Log::logW("MeshComponent::getMesh called while no mesh is stored");

	return mesh;
}
