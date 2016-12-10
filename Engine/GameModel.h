#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include "Shader.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "misc/Vertex.h"
#include "Mesh.h"
#include "AABB.h"

//Forward Declaration
class ResourceManager;


/// Class to store and display a model
class GameModel : public Resource
{
public:
	GameModel(std::weak_ptr<aiScene> scene);

	GameModel(
		std::vector<glm::vec3>* vertices,
		std::vector<glm::vec3>* normals,
		std::vector<glm::vec2>* uvs,
		std::vector<unsigned int>* indices);
	
	~GameModel();

	GLuint getVAO();

	bool hasIndexBuffer();

	unsigned int getNumVertices();

	unsigned int getNumIndices();

	unsigned int getMeshCount();

	Mesh getSubmesh(unsigned int index);

	AABB getAABB();
private:

	/// Vertex Array Object for model in OpenGL
	GLuint VAO;

	/** @brief Buffers for vertex data. */
	std::vector<GLuint> VBOs;

	/// Buffer for indices
	GLuint indexBuffer;
	
	void initMeshFromAssimp(
		std::weak_ptr<aiMesh> meshPtr,
		std::vector<glm::vec3>& positions,
		std::vector<glm::vec3>& normals,
		std::vector<glm::vec2>& uvs,
		std::vector<glm::vec3>& tangents,
		std::vector<glm::vec3>& biTangents,
		std::vector<unsigned int>& indicies
	);


	void addVBO(std::vector<glm::vec3> &data);
	void addVBO(std::vector<glm::vec2> &data);

	void addIndexBuffer(std::vector<unsigned int> &indices);

	
	void processAssimpScene(std::weak_ptr<aiScene> scenePtr);

	std::vector<unsigned int> extractMeshIndexData(std::weak_ptr<aiMesh> meshPtr);

	/// Number of vertices in the model
	unsigned int numVertices;

	std::vector<Mesh> meshes;


	unsigned int numIndices;

	AABB bounds;

	void calculateAABB(std::vector<glm::vec3> vertices);
};