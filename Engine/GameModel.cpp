#include "GameModel.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "misc/Vertex.h"
#include "misc/Utility.h"
#include "misc/Log.h"



GameModel::GameModel(const aiScene* scene)
{
	indexBuffer = 0;
	numVertices = 0;
	numIndices = 0;

	glGenVertexArrays(1, &VAO);

	processAssimpScene(scene);
}

GameModel::GameModel(std::vector<glm::vec3>* vertices, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uvs,
	std::vector<unsigned int>* indices) : Resource()
{
	// Initialise variables
	indexBuffer = 0;
	numVertices = 0;
	numIndices = 0;

	// Creates one VAO
	glGenVertexArrays(1, &VAO);

	// Create the model
	if (vertices == nullptr)
	{
		Log::logW("A GameModel was loaded without any verticies");
		assert(false);
		return;
	}

	numVertices = vertices->size();
	addVBO(*vertices);

	if (normals != nullptr) 
		addVBO(*normals);

	if (uvs != nullptr)
		addVBO(*uvs);

	if (indices != nullptr)
		addIndexBuffer(*indices);
}

GameModel::~GameModel()
{
	//Just incase the below VAO deletion doesn't cover it, delete all other data
	for (auto VBO : VBOs)
	{
		glDeleteBuffers(1, &VBO);
	}
	VBOs.clear();

	//I believe this will delete all associated data, as once a VBO/Texture has nothing referencing it, it will be deleted
	glDeleteVertexArrays(1, &VAO);

}

std::vector<unsigned int> GameModel::extractMeshIndexData(aiMesh* mesh)
{
	std::vector<unsigned int> indexArray;
	
	//Loop through every face
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		//Check that the face is a triangle
		assert(mesh->mFaces[i].mNumIndices == 3);
		
		//Add the 3 indices to the array
		indexArray.push_back(mesh->mFaces[i].mIndices[0]);
		indexArray.push_back(mesh->mFaces[i].mIndices[1]);
		indexArray.push_back(mesh->mFaces[i].mIndices[2]);
	}

	return indexArray;
}

void GameModel::calculateAABB(std::vector<glm::vec3> vertices)
{
	if (vertices.size() < 1)
	{
		bounds = AABB(glm::vec3(0), glm::vec3(0));
		return;
	}


	glm::vec3 min(vertices[0]);
	glm::vec3 max(min);

	for (glm::vec3 vertex : vertices)
	{
		//Rolled into loop for hopefully better performance
		for (unsigned int axis = 0; axis < 3; axis++)
		{
			if (vertex[axis] < min[axis])
			{
				min[axis] = vertex[axis];
			}
			else if (vertex[axis] > max[axis])
			{
				max[axis] = vertex[axis];
			}
		}
	}

	glm::vec3 center = (max + min) / 2.0f;
	glm::vec3 size = (max - min);

	bounds = AABB(center, size);
}

void GameModel::processAssimpScene(const aiScene* scene)
{
	assert(scene->HasMeshes());

	//Pre allocate space
	meshes.reserve(scene->mNumMeshes);

	//Counts
	numVertices = 0;
	numIndices = 0;

	//Get initial information
	for (unsigned int currentMeshIndex = 0; currentMeshIndex < scene->mNumMeshes; currentMeshIndex++)
	{
		auto mesh = scene->mMeshes[currentMeshIndex];

		//Format checks
		if (mesh->GetNumUVChannels() > 1)
			Log::logW("Mesh has unsupported number of UV channels, this can lead to undefined behaviour");

		assert(
			mesh->HasPositions() &&
			mesh->HasNormals() &&
			mesh->HasFaces() &&
			mesh->HasTextureCoords(0) &&
			mesh->HasTangentsAndBitangents() &&
			mesh->mFaces[0].mNumIndices == 3
		);

		meshes.push_back(Mesh(
			mesh->mNumFaces * 3,
			numVertices,
			numIndices,
			mesh->mMaterialIndex
		));
		
		numVertices += mesh->mNumVertices;
		numIndices += meshes[currentMeshIndex].numIndicies;
	}


	//Reserve Space in arrays (This saves a lot of processing time, thats why I do two loops)
	std::vector<glm::vec3> positions;   positions.reserve(numVertices);
	std::vector<glm::vec3> normals;		normals.reserve(numVertices);
	std::vector<glm::vec2> uvs;			uvs.reserve(numVertices);
	std::vector<glm::vec3> tangents;	tangents.reserve(numVertices);
	std::vector<glm::vec3> biTangents;  biTangents.reserve(numVertices);

	std::vector<unsigned int> indices;  indices.reserve(numIndices);

	for (unsigned int currentMeshIndex = 0; currentMeshIndex < scene->mNumMeshes; currentMeshIndex++)
	{
		aiMesh* mesh = scene->mMeshes[currentMeshIndex];
		
		initMeshFromAssimp(
			mesh,
			positions,
			normals,
			uvs,
			tangents,
			biTangents,
			indices
		);
	}
	
	//Fill Buffers
	glBindVertexArray(VAO);

	addVBO(positions);
	addVBO(normals);
	addVBO(uvs);
	addVBO(tangents);
	addVBO(biTangents);

	addIndexBuffer(indices);

	//Calculate AABB bounds
	calculateAABB(positions);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameModel::initMeshFromAssimp(aiMesh* mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, 
	std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& biTangents, std::vector<unsigned int>& indicies)
{
	//Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D position = mesh->mVertices[i];
		const aiVector3D normal = mesh->mNormals[i];
		const aiVector3D uv = mesh->mTextureCoords[0][i];
		const aiVector3D tangent = mesh->mTangents[i];
		const aiVector3D biTangent = mesh->mBitangents[i];

		positions.push_back(glm::vec3(position.x, position.y, position.z));
		normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
		uvs.push_back(glm::vec2(uv.x, uv.y));
		tangents.push_back(glm::vec3(tangent.x, tangent.y, tangent.z));
		biTangents.push_back(glm::vec3(biTangent.x, biTangent.y, biTangent.z));
	}

	//Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indicies.push_back(face.mIndices[0]);
		indicies.push_back(face.mIndices[1]);
		indicies.push_back(face.mIndices[2]);
	}
}

void GameModel::addVBO(std::vector<glm::vec3> &data)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	VBOs.push_back(VBO);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);

	//Enable the next available attrib array which is equal to (the number of VBOS - 1)
	GLuint attribArrayID = VBOs.size() - 1;
	glEnableVertexAttribArray(attribArrayID);
	glVertexAttribPointer(
		attribArrayID, //Attrib ID
		3,			   //Size
		GL_FLOAT,      //Type
		GL_FALSE,      //Normalized?
		0,             //Stride
		(void*)0       //Array Buffer Offset
	);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameModel::addVBO(std::vector<glm::vec2> &data)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	VBOs.push_back(VBO);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), &data[0], GL_STATIC_DRAW);

	//Enable the next available attrib array which is equal to (the number of VBOS - 1)
	GLuint attribArrayID = VBOs.size() - 1;
	glEnableVertexAttribArray(attribArrayID);
	glVertexAttribPointer(
		attribArrayID, //Attrib ID
		2,			   //Size
		GL_FLOAT,      //Type
		GL_FALSE,      //Normalized?
		0,             //Stride
		(void*)0       //Array Buffer Offset
	);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void GameModel::addIndexBuffer(std::vector<unsigned int> &indices)
{
	numIndices = indices.size();

	//Create and Bind Index Buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint GameModel::getVAO()
{
	return VAO;
}

bool GameModel::hasIndexBuffer()
{
	return (indexBuffer != 0);
}

unsigned int GameModel::getNumVertices()
{
	return numVertices;
}

unsigned int GameModel::getNumIndices()
{
	return numIndices;
}

unsigned int GameModel::getMeshCount()
{
	return meshes.size();
}

Mesh GameModel::getSubmesh(unsigned int index)
{
	return meshes[index];
}

AABB GameModel::getAABB()
{
	return bounds;
}
