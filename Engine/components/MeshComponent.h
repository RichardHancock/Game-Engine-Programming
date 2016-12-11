#pragma once

#include "Component.h"

#include "../GameModel.h"

class MeshComponent : public Component
{
public:

	virtual ~MeshComponent();

	void setMesh(std::weak_ptr<GameModel> newMesh);

	std::weak_ptr<GameModel> getMesh();

private:
	std::weak_ptr<GameModel> mesh;
};