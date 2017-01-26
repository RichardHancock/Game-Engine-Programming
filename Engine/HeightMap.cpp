#include "HeightMap.h"

#include "misc/Log.h"
#include "misc/Utility.h"

std::shared_ptr<GameModel> HeightMap::load(std::string imgPath, float heightScale, float xyScale)
{
	Texture* mapWrap = new Texture(imgPath, false);

	SDL_Surface* map = mapWrap->getSurface();

	if (map->format->BytesPerPixel != 1)
	{
		Log::logE(imgPath + " is not 8 bit. Height map generator only accepts 8 bit images such as greyscale.");
		return nullptr;
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;
	
	calculateVertices(vertices, map, heightScale, xyScale);
	calculateNormals(normals, vertices, map);
	calculateUVs(uvs, map);
	calculateIndices(indices, map);

	return std::make_shared<GameModel>(&vertices, &normals, &uvs, &indices);
}

void HeightMap::calculateVertices(std::vector<glm::vec3>& vertices, SDL_Surface * map,
	float heightScale, float xyScale)
{
	int width = map->w;
	int height = map->h;

	vertices.reserve(width * height);

	int index;

	//Loop through every pixel in the image
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			index = y*width + x; //Calculate current pixel index address

			Uint8 pixel = *((Uint8*)map->pixels + index);

			//Fetch the colour data and scale it to a reasonable size, also sets the x and y positions
			vertices.push_back(glm::vec3(x * xyScale, (float)pixel * heightScale, y * xyScale));
		}
	}
}

void HeightMap::calculateNormals(std::vector<glm::vec3>& normals, std::vector<glm::vec3>& vertices, SDL_Surface* map)
{
	int width = map->w;
	int height = map->h;

	int pixelCount = (height * width);

	normals.reserve(pixelCount);

	// Loop through every pixel in the image apart from the bottom row.
	for (int pixIndex = 0; pixIndex < pixelCount; pixIndex++)
	{
		// Skip the final X coordinates for each row to avoid extra triangles.
		if (((pixIndex + 1) % width != 0 && pixIndex < width * (height - 1)) || pixIndex == 0)
		{
			normals.push_back(Utility::calculateFaceNormal(vertices[pixIndex], vertices[pixIndex + width], vertices[pixIndex + width + 1]));
		}
		else if (pixIndex >= width * (height - 1))
		{
			normals.push_back(vertices[pixIndex - width]);
		}
		else
		{
			normals.push_back(vertices[pixIndex - 1]);
		}
	}
}

void HeightMap::calculateUVs(std::vector<glm::vec2>& uvs, SDL_Surface* map)
{
	int width = map->w;
	int height = map->h;

	uvs.reserve(width * height);

	//Loop through every pixel in the image
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = y*width + x; //Calculate current pixel index address

			glm::vec2 uv;
			uv.x = 1.0f - Utility::normaliseFloat((float)x, 0, width - 1);
			uv.y = Utility::normaliseFloat((float)y, 0, height - 1);

			uvs.push_back(uv);
		}
	}
}

void HeightMap::calculateIndices(std::vector<unsigned int>& indices, SDL_Surface* map)
{
	int width = map->w;
	int height = map->h;

	int pixelCount = ((height - 1) * width) - 1;
	indices.reserve(pixelCount);

	// Loop through every pixel in the image apart from the bottom row.
	for (int pixIndex = 0; pixIndex < pixelCount; pixIndex++)
	{
		// Skip the final X coordinates for each row to avoid extra triangles.
		if ((pixIndex + 1) % width != 0 | pixIndex == 0)
		{
			// Calculates the first Triangle of the square.
			indices.push_back(pixIndex); // Top Left
			indices.push_back(pixIndex + width); // Bottom Left
			indices.push_back(pixIndex + width + 1); // Bottom Right

											// Calculates the second Triangle of the square.
			indices.push_back(pixIndex); // Top Left
			indices.push_back(pixIndex + width + 1); // Bottom Right
			indices.push_back(pixIndex + 1); // Top Right
		}
	}
}
