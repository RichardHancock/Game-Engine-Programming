#pragma once

#include <memory>

#include "GameModel.h"
#include "Texture.h"

class HeightMap
{
public:
	static std::shared_ptr<GameModel> load(std::string imgPath, float heightScale, float xyScale);

private:

	static void calculateVertices(std::vector<glm::vec3>& vertices, SDL_Surface* map, 
		float heightScale, float xyScale);

	static void calculateNormals(std::vector<glm::vec3>& normals, std::vector<glm::vec3>& vertices, SDL_Surface* map);

	static void calculateUVs(std::vector<glm::vec2>& uvs, SDL_Surface* map);

	static void calculateIndices(std::vector<unsigned int>& indices, SDL_Surface* map);
};