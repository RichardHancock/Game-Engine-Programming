#include "GameModel.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "misc/Vertex.h"
#include "misc/Utility.h"
#include "misc/Log.h"

GameModel::GameModel(aiMesh* mesh)
	: Resource()
{
	// Initialise variables
	indexBuffer = 0;
	numVertices = 0;
	numIndices = 0;
	nextAvailableTextureUnit = GL_TEXTURE0;
	disableMatUniforms = false;
	
	// Creates one VAO
	glGenVertexArrays(1, &VAO);

	// Create the model
	loadModelDataFromASSIMP(mesh);
}

GameModel::GameModel()
{
	indexBuffer = 0;
	numVertices = 0;
	numIndices = 0;
	nextAvailableTextureUnit = GL_TEXTURE0;
	disableMatUniforms = false;
}

GameModel::GameModel(std::vector<glm::vec3>* vertices, std::vector<glm::vec3>* normals, std::vector<glm::vec2>* uvs,
	std::vector<unsigned int>* indices) : Resource()
{
	// Initialise variables
	indexBuffer = 0;
	numVertices = 0;
	numIndices = 0;
	nextAvailableTextureUnit = GL_TEXTURE0;
	disableMatUniforms = false;

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

	for (auto texture : textures)
	{
		//glDeleteTextures(1, &texture.textureID);
	}
	textures.clear();
	

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

void GameModel::loadModelDataFromASSIMP(aiMesh* mesh)
{
	glBindVertexArray(VAO);

	//Check that the model is valid
	assert(
		mesh->HasNormals() &&
		mesh->HasFaces() &&
		mesh->HasPositions() &&
		mesh->HasTangentsAndBitangents() &&
		mesh->HasTextureCoords(0)
	);

	// Vertex + Normal + TexCoord + Tangent + biTangent
	const int VERTEX_ATTRIBUTE_COUNT = (3 + 3 + 2 + 3 + 3);
	numVertices = mesh->mNumVertices;

	aiVector3D* vertices = mesh->mVertices;
	aiVector3D* normals = mesh->mNormals;
	aiVector3D* UVs = mesh->mTextureCoords[0]; //We only support 2D textures so the Z is ignored
	aiVector3D* tangents = mesh->mTangents;
	aiVector3D* biTangents = mesh->mBitangents;

	GLfloat* vertexData = new GLfloat[numVertices * VERTEX_ATTRIBUTE_COUNT];

	//Interleave the data into a array (Possible performance boost (Or loss, research isn't very conclusive))
	for (unsigned int inputIndex = 0, outputIndex = 0;
		inputIndex < numVertices; 
		inputIndex++, outputIndex += VERTEX_ATTRIBUTE_COUNT)
	{
		vertexData[outputIndex     ] = vertices[inputIndex].x;
		vertexData[outputIndex + 1 ] = vertices[inputIndex].y;
		vertexData[outputIndex + 2 ] = vertices[inputIndex].z;
								   
		vertexData[outputIndex + 3 ] = normals[inputIndex].x;
		vertexData[outputIndex + 4 ] = normals[inputIndex].y;
		vertexData[outputIndex + 5 ] = normals[inputIndex].z;
								   
		vertexData[outputIndex + 6 ] = UVs[inputIndex].x;
		vertexData[outputIndex + 7 ] = UVs[inputIndex].y;
								   
		vertexData[outputIndex + 8 ] = tangents[inputIndex].x;
		vertexData[outputIndex + 9 ] = tangents[inputIndex].y;
		vertexData[outputIndex + 10] = tangents[inputIndex].z;

		vertexData[outputIndex + 11] = biTangents[inputIndex].x;
		vertexData[outputIndex + 12] = biTangents[inputIndex].y;
		vertexData[outputIndex + 13] = biTangents[inputIndex].z;
	}

	//Create and Bind VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	VBOs.push_back(VBO);

	//Pass Data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (numVertices * VERTEX_ATTRIBUTE_COUNT), vertexData, GL_STATIC_DRAW);

	//Tell OpenGL how the data is interleaved (the stride between the data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AdvVertex), (void*)(0 + offsetof(AdvVertex, v)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AdvVertex), (void*)(0 + offsetof(AdvVertex, vn)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AdvVertex), (void*)(0 + offsetof(AdvVertex, vt)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AdvVertex), (void*)(0 + offsetof(AdvVertex, tan)));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AdvVertex), (void*)(0 + offsetof(AdvVertex, bitan)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);


	//Indicies
	std::vector<unsigned int> indices = extractMeshIndexData(mesh);
	
	addIndexBuffer(indices);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameModel::processAssimpScene(const aiScene * scene)
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
		auto mesh = scene->mMeshes[currentMeshIndex];
		initMeshFromAssimp(mesh,
			positions,
			normals,
			uvs,
			tangents,
			biTangents,
			indices
		);
	}
	
	//Fill Buffers
	glGenVertexArrays(1, &VAO);
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

void GameModel::initMeshFromAssimp(aiMesh * mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& biTangents, std::vector<unsigned int>& indicies)
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

/*
void GameModel::addTexture(Texture* texture, std::string shaderVarName)
{
	glBindVertexArray(VAO);

	GLenum textureFormat = 0;

	SDL_Surface* surface = texture->getRawSurface();


	//Section from http://ubuntuforums.org/archive/index.php/t-1088836.html post 2
#ifdef _MSC_VER
	__pragma(warning(push));
	__pragma(warning(disable:4127)); // This is a problem in SDL2 (Can't fix) and isn't any danger
#endif // _MSC_VER

	switch (surface->format->BytesPerPixel) {
	case 4:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGRA;
		else
			textureFormat = GL_RGBA;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			textureFormat = GL_BGR;
		else
			textureFormat = GL_RGB;
		break;
	}

#ifdef _MSC_VER
	__pragma(warning(pop));
#endif // _MSC_VER
	//End Section

	assert(textureFormat != 0);

	TextureWrapper newTexture;
	//Get Current texture unit and then cycle it to the next one
	newTexture.textureUnit = nextAvailableTextureUnit++;
	newTexture.texture = texture;
	newTexture.shaderVarName = shaderVarName;

	glGenTextures(1, &newTexture.textureID);
	glBindTexture(GL_TEXTURE_2D, newTexture.textureID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		textureFormat,
		texture->getRawSurface()->w,
		texture->getRawSurface()->h,
		0,
		textureFormat,
		GL_UNSIGNED_BYTE,
		texture->getRawSurface()->pixels
		);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	textures.push_back(newTexture);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void GameModel::deleteTexturesFromGPU()
{
	glBindVertexArray(VAO);

	for (auto texture : textures)
	{
		glDeleteTextures(1, &texture.textureID);
	}
	textures.clear();

	glBindVertexArray(0);
}
*/

void GameModel::draw(glm::mat4& modelMatrix, glm::mat4& viewMatrix, glm::mat4& projMatrix, Shader* shader)
{
	glUseProgram(shader->getProgram());

		glBindVertexArray( VAO );

			if (!disableMatUniforms)
			{
				shader->setUniform("modelMat", modelMatrix);
				shader->setUniform("viewMat", viewMatrix);
				shader->setUniform("projMat", projMatrix);
			}

			
			//Textures
			unsigned int currentUnit = 0;
			for (auto texture : textures)
			{
				//TODO texture unit needs to change
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture.textureID);
				
				shader->setUniform(texture.shaderVarName, currentUnit);

				currentUnit++;
			}
			

			if (indexBuffer != 0) 
			{
				glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*) 0);
			}
			else 
			{
				glDrawArrays(GL_TRIANGLES, 0, numVertices);
			}
		
		glBindVertexArray( 0 );
	
	glUseProgram( 0 );
}

void GameModel::draw2D(Shader* shader)
{
	glUseProgram(shader->getProgram());

		glBindVertexArray(VAO);

			//Textures
			unsigned int currentUnit = 0;
			for (auto texture : textures)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture.textureID);

				shader->setUniform(texture.shaderVarName, currentUnit);
				shader->setUniform("textureFlag", 1.0f);
				currentUnit++;
			}

		
			if (indexBuffer != 0) 
			{
				glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
			}
			else 
			{
				glDrawArrays(GL_TRIANGLES, 0, numVertices);
			}

		glBindVertexArray(0);

	glUseProgram(0);
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
