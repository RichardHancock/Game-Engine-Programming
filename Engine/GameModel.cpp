#include "GameModel.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "misc/Vertex.h"
#include "misc/Utility.h"
#include "misc/Log.h"
#include "Platform.h"



GameModel::GameModel(const aiScene* scene)
{
	indexBuffer = 0;
	numVertices = 0;
	numIndices = 0;
	VAO = 0;

	processAssimpScene(scene);
}

GameModel::GameModel(std::vector<glm::vec3>* inVertices, std::vector<glm::vec3>* inNormals, std::vector<glm::vec2>* inUvs,
	std::vector<unsigned int>* inIndices) : Resource()
{
	// Initialise variables
	indexBuffer = 0;
	numVertices = 0;
	numIndices = 0;
	VAO = 0;

	
	// Create the model
	if (inVertices == nullptr)
	{
		Log::logW("A GameModel was loaded without any vertices");
		assert(false);
		return;
	}

	numVertices = inVertices->size();
	numIndices = 0;
	
	if (!Platform::isDummyRenderer())
	{
		// Creates one VAO
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		addVBO(*inVertices);
		vertices = *inVertices;

		if (inNormals != nullptr)
			addVBO(*inNormals);

		if (inUvs != nullptr)
			addVBO(*inUvs);

		if (inIndices != nullptr)
		{
			addIndexBuffer(*inIndices);
			numIndices = inIndices->size();
			indices = *inIndices;
		}

		meshes.push_back(Mesh(
			numIndices,
			0,
			0,
			0
		));

		calculateAABB(*inVertices);

		glBindVertexArray(0);
	}
}

GameModel::GameModel(std::vector<Vertex> advVertices)
{
	indexBuffer = 0;
	numVertices = 0;
	numIndices = 0;
	VAO = 0;
	
	initModelFromAdvVertices(advVertices);
}

std::shared_ptr<GameModel> GameModel::getCubeModel()
{
	std::vector<glm::vec3> vertices =
	{
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		
		glm::vec3(-1.0f, -1.0f,  1.0),
		glm::vec3(-1.0f,  1.0f,  1.0),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f)
	};

	std::shared_ptr<GameModel> cube = std::make_shared<GameModel>(&vertices, nullptr, nullptr, nullptr);

	return cube;
}

GameModel::~GameModel()
{
	//Just in case the below VAO deletion doesn't cover it, delete all other data
	for (auto VBO : VBOs)
	{
		glDeleteBuffers(1, &VBO);
	}
	VBOs.clear();

	//I believe this will delete all associated data, as once a VBO has nothing referencing it, it will be deleted
	if (VAO != 0)
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
		numIndices += meshes[currentMeshIndex].numIndices;
	}


	//Reserve Space in arrays (This saves a lot of processing time, that's why I do two loops)
	std::vector<glm::vec3> positions;   positions.reserve(numVertices);
	std::vector<glm::vec3> normals;		normals.reserve(numVertices);
	std::vector<glm::vec2> uvs;			uvs.reserve(numVertices);
	std::vector<glm::vec3> tangents;	tangents.reserve(numVertices);
	std::vector<glm::vec3> biTangents;  biTangents.reserve(numVertices);

	indices.reserve(numIndices);

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

	//Calculate AABB bounds
	calculateAABB(positions);

	//OpenGL Buffer Creation
	if (!Platform::isDummyRenderer())
	{
		glGenVertexArrays(1, &VAO);

		//Fill Buffers
		glBindVertexArray(VAO);

		addVBO(positions);
		addVBO(normals);
		addVBO(uvs);
		addVBO(tangents);
		addVBO(biTangents);

		addIndexBuffer(indices);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	vertices = positions;
}

void GameModel::initMeshFromAssimp(aiMesh* mesh, std::vector<glm::vec3>& inPositions, std::vector<glm::vec3>& inNormals, 
	std::vector<glm::vec2>& inUvs, std::vector<glm::vec3>& inTangents, std::vector<glm::vec3>& inBiTangents, std::vector<unsigned int>& inIndices)
{
	//Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D position = mesh->mVertices[i];
		const aiVector3D normal = mesh->mNormals[i];
		const aiVector3D uv = mesh->mTextureCoords[0][i];
		const aiVector3D tangent = mesh->mTangents[i];
		const aiVector3D biTangent = mesh->mBitangents[i];

		inPositions.push_back(glm::vec3(position.x, position.y, position.z));
		inNormals.push_back(glm::vec3(normal.x, normal.y, normal.z));
		inUvs.push_back(glm::vec2(uv.x, uv.y));
		inTangents.push_back(glm::vec3(tangent.x, tangent.y, tangent.z));
		inBiTangents.push_back(glm::vec3(biTangent.x, biTangent.y, biTangent.z));
	}

	//Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		inIndices.push_back(face.mIndices[0]);
		inIndices.push_back(face.mIndices[1]);
		inIndices.push_back(face.mIndices[2]);
	}
}

void GameModel::initModelFromAdvVertices(std::vector<Vertex> advVertices)
{
	const int vertexCount = advVertices.size() * 8;
	numVertices = advVertices.size();
	std::vector<GLfloat> vertexData;
	vertexData.resize(vertexCount);
	
	for (unsigned int i = 0; i < advVertices.size(); i++)
	{
		unsigned int aPos = i * 8; // Position in the GLfloat array

		vertexData[aPos] =	   advVertices[i].v.x;
		vertexData[aPos + 1] = advVertices[i].v.y;
		vertexData[aPos + 2] = advVertices[i].v.z;

		vertexData[aPos + 3] = advVertices[i].vn.x;
		vertexData[aPos + 4] = advVertices[i].vn.y;
		vertexData[aPos + 5] = advVertices[i].vn.z;

		vertexData[aPos + 6] = advVertices[i].vt.x;
		vertexData[aPos + 7] = advVertices[i].vt.y;

		vertices.push_back(Utility::vec3ToGLM(advVertices[i].v));
	}

	meshes.push_back(Mesh(
		0,
		0,
		0,
		0
	));

	//Calculate AABB bounds
	calculateAABB(advVertices);

	//OpenGL Buffer Creation
	if (!Platform::isDummyRenderer())
	{
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		GLuint vertexBuffer = 0;

		glGenBuffers(1, &vertexBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, &vertexData[0], GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, v)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vn)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 + offsetof(Vertex, vt)));

		VBOs.push_back(vertexBuffer);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void GameModel::addVBO(std::vector<glm::vec3> &data)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	VBOs.push_back(VBO);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);

	//Enable the next available attribute array which is equal to (the number of VBOS - 1)
	GLuint attribArrayID = VBOs.size() - 1;
	glEnableVertexAttribArray(attribArrayID);
	glVertexAttribPointer(
		attribArrayID, //Attribute ID
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

	//Enable the next available attribute array which is equal to (the number of VBOS - 1)
	GLuint attribArrayID = VBOs.size() - 1;
	glEnableVertexAttribArray(attribArrayID);
	glVertexAttribPointer(
		attribArrayID, //Attribute ID
		2,			   //Size
		GL_FLOAT,      //Type
		GL_FALSE,      //Normalized?
		0,             //Stride
		(void*)0       //Array Buffer Offset
	);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void GameModel::addIndexBuffer(std::vector<unsigned int> &inIndices)
{
	numIndices = inIndices.size();

	//Create and Bind Index Buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &inIndices[0], GL_STATIC_DRAW);

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

void GameModel::calculateAABB(std::vector<glm::vec3> inVertices)
{
	if (inVertices.size() < 1)
	{
		bounds = AABB(glm::vec3(0), glm::vec3(0));
		return;
	}


	glm::vec3 min(inVertices[0]);
	glm::vec3 max(min);

	for (glm::vec3 vertex : inVertices)
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

void GameModel::calculateAABB(std::vector<Vertex> advVertices)
{
	if (advVertices.size() < 1)
	{
		bounds = AABB(glm::vec3(0), glm::vec3(0));
		return;
	}


	Vec3 min(advVertices[0].v);
	Vec3 max(min);

	for (Vertex vertex : advVertices)
	{
		//X
		if (vertex.v.x < min.x)
		{
			min.x = vertex.v.x;
		}
		else if (vertex.v.x > max.x)
		{
			max.x = vertex.v.x;
		}
		//Y
		if (vertex.v.y < min.y)
		{
			min.y = vertex.v.y;
		}
		else if (vertex.v.y > max.y)
		{
			max.y = vertex.v.y;
		}
		//Z
		if (vertex.v.z < min.z)
		{
			min.z = vertex.v.z;
		}
		else if (vertex.v.z > max.z)
		{
			max.z = vertex.v.z;
		}
	}

	Vec3 center = (max + min) / 2.0f;
	Vec3 size = (max - min);

	bounds = AABB(Utility::vec3ToGLM(center), Utility::vec3ToGLM(size));
}