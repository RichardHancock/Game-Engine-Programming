#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include "Resource.h"
#include "misc/Vertex.h"
#include "Mesh.h"
#include "misc/AABB.h"

//Forward Declaration
class ResourceManager;


/// Class to store a model
class GameModel : public Resource
{
public:

	/**
	 @brief	Constructor.
	
	 @param	scene	The Assimp scene.
	 */
	GameModel(const aiScene* scene);

	/**
	 @brief	Constructor.
	
	 @param [in,out]	vertices	If non-null, the vertices.
	 @param [in,out]	normals 	If non-null, the normals.
	 @param [in,out]	uvs			If non-null, the uvs.
	 @param [in,out]	indices 	If non-null, the indices.
	 */
	GameModel(
		std::vector<glm::vec3>* vertices,
		std::vector<glm::vec3>* normals,
		std::vector<glm::vec2>* uvs,
		std::vector<unsigned int>* indices);

	/**
	 @brief	Constructor.
	
	 @param	advVertices	The advance vertices from OBJ Loader.
	 */
	GameModel(std::vector<Vertex> advVertices);

	/** @brief	Destructor. */
	~GameModel();

	/**
	 @brief	Gets the VAO.
	
	 @return	The VAO.
	 */
	GLuint getVAO();

	/**
	 @brief	Query if this GameModel has a index buffer.
	
	 @return	true if index buffer, false if not.
	 */
	bool hasIndexBuffer();

	/**
	 @brief	Gets number vertices.
	
	 @return	The number vertices.
	 */
	unsigned int getNumVertices();

	/**
	 @brief	Gets number indices.
	
	 @return	The number indices.
	 */
	unsigned int getNumIndices();

	/**
	 @brief	Gets mesh count.
	
	 @return	The mesh count.
	 */
	unsigned int getMeshCount();

	/**
	 @brief	Gets a submesh.
	
	 @param	index	Zero-based index of the submesh.
	
	 @return	The submesh.
	 */
	Mesh getSubmesh(unsigned int index);

	/**
	 @brief	Gets Axis Aligned Bounding Box
	
	 @return	AABB.
	 */
	AABB getAABB();
private:

	/// Vertex Array Object for model in OpenGL
	GLuint VAO;

	/** @brief Buffers for vertex data. */
	std::vector<GLuint> VBOs;

	/// Buffer for indices
	GLuint indexBuffer;

	/**
	 @brief	Init sub mesh from assimp.
	
	 @param [in,out]	mesh	  	If non-null, the mesh.
	 @param [in,out]	positions 	The positions.
	 @param [in,out]	normals   	The normals.
	 @param [in,out]	uvs		  	The uvs.
	 @param [in,out]	tangents  	The tangents.
	 @param [in,out]	biTangents	The bi tangents.
	 @param [in,out]	indicies  	The indicies.
	 */
	void initMeshFromAssimp(
		aiMesh* mesh,
		std::vector<glm::vec3>& positions,
		std::vector<glm::vec3>& normals,
		std::vector<glm::vec2>& uvs,
		std::vector<glm::vec3>& tangents,
		std::vector<glm::vec3>& biTangents,
		std::vector<unsigned int>& indicies
	);

	/**
	 @brief	Init model from advance vertices from OBJ Loader.
	
	 @param	advVertices	The advance vertices.
	 */
	void initModelFromAdvVertices(std::vector<Vertex> advVertices);

	/**
	 @brief	Adds a VBO.
	
	 @param [in,out]	data	The data.
	 */
	void addVBO(std::vector<glm::vec3> &data);

	/**
	 @brief	Adds a VBO.
	
	 @param [in,out]	data	The data.
	 */
	void addVBO(std::vector<glm::vec2> &data);

	/**
	 @brief	Adds an index buffer.
	
	 @param [in,out]	indices	The indices.
	 */
	void addIndexBuffer(std::vector<unsigned int> &indices);

	/**
	 @brief	Process the assimp scene described by scene.
	
	 @param	scene	The scene.
	 */
	void processAssimpScene(const aiScene* scene);

	/**
	 @brief	Extracts the mesh index data described by mesh.
	 Converts Assimp faces into uints
	 @param [in,out]	mesh	If non-null, the mesh.
	
	 @return	The extracted mesh index data.
	 */
	std::vector<unsigned int> extractMeshIndexData(aiMesh* mesh);

	/// Number of vertices in the model
	unsigned int numVertices;

	/** @brief	The submeshes. */
	std::vector<Mesh> meshes;


	/** @brief	Number of indices. */
	unsigned int numIndices;

	/** @brief	The Axis Aligned Bounding Box. */
	AABB bounds;

	/**
	 @brief	Calculates Axis Aligned Bounding Box.
	
	 @param	vertices	The vertices.
	 */
	void calculateAABB(std::vector<glm::vec3> vertices);

	/**
	@brief	Calculates Axis Aligned Bounding Box.

	@param	advVertices	The advanced vertices.
	*/
	void calculateAABB(std::vector<Vertex> advVertices);
};