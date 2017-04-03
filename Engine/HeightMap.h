#pragma once

#include <memory>

#include "GameModel.h"
#include "Texture.h"

/** @brief	A height map. */
class HeightMap
{
public:

	/**
	 @brief	Generate a height map.
	
	 @param	imgPath	   	Full pathname of the image file to use for height map data.
	 @param	heightScale	The height scale.
	 @param	xyScale	   	The XY scale.
	
	 @return	A game model containing the height map;
	 */
	static std::shared_ptr<GameModel> load(std::string imgPath, float heightScale, float xyScale);

private:

	/**
	 @brief	Calculates the vertices.
	
	 @param [in,out]	vertices   	The vertices.
	 @param [in,out]	map		   	If non-null, the map.
	 @param 			heightScale	The height scale.
	 @param 			xyScale	   	The xy scale.
	 */
	static void calculateVertices(std::vector<glm::vec3>& vertices, SDL_Surface* map, 
		float heightScale, float xyScale);

	/**
	 @brief	Calculates the normals.
	
	 @param [in,out]	normals 	The normals.
	 @param [in,out]	vertices	The vertices.
	 @param [in,out]	map			If non-null, the map.
	 */
	static void calculateNormals(std::vector<glm::vec3>& normals, std::vector<glm::vec3>& vertices, SDL_Surface* map);

	/**
	 @brief	Calculates the UVs.
	
	 @param [in,out]	uvs	The uvs.
	 @param [in,out]	map	If non-null, the map.
	 */
	static void calculateUVs(std::vector<glm::vec2>& uvs, SDL_Surface* map);

	/**
	 @brief	Calculates the indices.
	
	 @param [in,out]	indices	The indices.
	 @param [in,out]	map	   	If non-null, the map.
	 */
	static void calculateIndices(std::vector<unsigned int>& indices, SDL_Surface* map);
};