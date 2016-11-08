#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "GameModel.h"

class Graphics
{
	static void renderMesh(std::weak_ptr<GameModel> mesh, glm::mat4 modelMat);
};