#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "GameModel.h"
#include "Material.h"

class Graphics
{
public:
	static void renderMesh(std::weak_ptr<GameModel> mesh, std::weak_ptr<Material> material);
};
