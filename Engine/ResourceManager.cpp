#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ResourceManager.h"
#include "Audio.h"
#include "misc/Log.h"

//Static Declarations
const std::string ResourceManager::modelDir = "resources/models/";
const std::string ResourceManager::audioDir = "resources/audio/";
const std::string ResourceManager::shaderDir = "resources/shaders/";
const std::string ResourceManager::textureDir = "resources/textures/";
const float ResourceManager::UPDATE_DELAY = 10.0f;
std::unordered_map<std::string, std::shared_ptr<GameModel>> ResourceManager::models;
std::unordered_map<std::string, std::shared_ptr<Audio>> ResourceManager::audio;
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::textures;
std::unordered_map<std::string, std::vector<std::shared_ptr<Material>>> ResourceManager::materials;

std::shared_ptr<Assimp::Importer> ResourceManager::modelImporter = std::make_shared<Assimp::Importer>();
Utility::SimpleTimer ResourceManager::updateDelayTimer(UPDATE_DELAY);


ResourceManager::ResourceManager() {}

void ResourceManager::cleanUp()
{
	/*
	for(auto model : models)
	{
		delete model.second;
	}*/
	models.clear();
	/*
	for (auto curAudio : audio)
	{
		delete curAudio.second;
	}*/
	audio.clear();
	/*
	for (auto texture : textures)
	{
		delete texture.second;
	}*/
	textures.clear();
}

std::weak_ptr<Audio> ResourceManager::getAudio(std::string audioFilename, bool isMusic, bool defaultPath)
{
	//Should we use the default path for this type of resource or just use the provided filename
	if (defaultPath) {
		audioFilename = audioDir + audioFilename;
	}
	
	//Is it already loaded
	if (audio.count(audioFilename) > 0)
	{
		//Get a weak_ptr of the resource
		return audio[audioFilename];
	}

	//Else load the resource and add it to the resourceManager's storage
	std::shared_ptr<Audio> audioFile;
			
	if (isMusic)
	{
		audioFile = std::make_shared<Music>(audioFilename);
	}
	else
	{
		audioFile = std::make_shared<SFX>(audioFilename);
	}

	audio[audioFilename] = audioFile;
	
	return audioFile;
}

void ResourceManager::loadMaterialsFromAssimp(std::string materialName, const aiScene* scene)
{
	materials[materialName].reserve(scene->mNumMaterials);

	for (unsigned int curMaterial = 0; curMaterial < scene->mNumMaterials; curMaterial++)
	{
		std::shared_ptr<Material> newMaterial = std::make_shared<Material>(
			shaderDir + "vertexNormal.shader", shaderDir + "fragmentNormal.shader");

		aiMaterial* materialData = scene->mMaterials[curMaterial];

		if (materialData->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			materialData->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL);
			newMaterial->addTexture("diffuseMap", getTexture(path.C_Str()));
		}

		if (materialData->GetTextureCount(aiTextureType_NORMALS))
		{
			aiString path;
			materialData->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL);
			newMaterial->addTexture("normalMap", getTexture(path.C_Str()));
		} 
		else if (materialData->GetTextureCount(aiTextureType_HEIGHT)) 
		{ 
			aiString path;
			materialData->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL);
			newMaterial->addTexture("normalMap", getTexture(path.C_Str()));
		}


		if (materialData->GetTextureCount(aiTextureType_SPECULAR))
		{
			aiString path;
			materialData->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL);
			newMaterial->addTexture("specularMap", getTexture(path.C_Str()));
		}

		materials[materialName].push_back(newMaterial);
	}
}

std::weak_ptr<Material> ResourceManager::getMaterial(std::string materialName, unsigned int index)
{
	materialName = modelDir + materialName;

	if (materials.count(materialName) > 0)
	{
		std::weak_ptr<Material> materialPtr;
		
		try {
			materialPtr = materials[materialName].at(index);
			return materialPtr;
		}
		catch (std::out_of_range)
		{
			return std::weak_ptr<Material>();
		}
	}

	return std::weak_ptr<Material>();
}

std::vector<std::weak_ptr<Material>> ResourceManager::getMaterials(std::string materialName)
{
	materialName = modelDir + materialName;

	if (materials.count(materialName) > 0)
	{
		std::vector<std::weak_ptr<Material>> result;
		result.reserve(materials[materialName].size());
		
		for (unsigned int curMaterial = 0; curMaterial < materials[materialName].size(); curMaterial++)
		{
			result.push_back(materials[materialName][curMaterial]);
		}

		return result;
	}

	return std::vector<std::weak_ptr<Material>>();
}

std::weak_ptr<GameModel> ResourceManager::getModel(std::string modelFilename, bool defaultPath)
{
	//Should we use the default path for this type of resource or just use the provided filename
	if (defaultPath) {
		modelFilename = modelDir + modelFilename;
	}

	//Is it already loaded
	if (models.count(modelFilename) > 0)
	{
		//Get a weak_ptr of the resource
		return models[modelFilename];
	}

	
	Log::logI("Loading model: " + modelFilename);

	//Read in the model file and apply Post processing flags
	int flags = (
		aiProcess_JoinIdenticalVertices | 
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals
		);
	//I tried to convert these into shared_ptrs but the internals of Assimp really do not suit it and was causing random memory errors.
	const aiScene* rawModelData = modelImporter->ReadFile(modelFilename, flags);

	if (rawModelData == nullptr)
	{
		Log::logE(modelFilename + " import failed: " + modelImporter->GetErrorString());
		return std::weak_ptr<GameModel>();
	}

	//Load Model
	std::shared_ptr<GameModel> modelData = std::make_shared<GameModel>(rawModelData);
	
	//Load Relevant Materials
	loadMaterialsFromAssimp(modelFilename, rawModelData);

	//We've converted the data to our formats so delete the raw version.
	modelImporter->FreeScene();

	//Store and return model
	models[modelFilename] = modelData;
	return modelData;
}

std::weak_ptr<Texture> ResourceManager::getTexture(std::string textureFilename, bool defaultPath)
{
	//Should we use the default path for this type of resource or just use the provided filename
	if (defaultPath) {
		textureFilename = textureDir + textureFilename;
	}

	//Is it already loaded
	if (textures.count(textureFilename) > 0)
	{
		//Get a weak_ptr of the resource
		return textures[textureFilename];
	}

	//Else load the resource and add it to the resourceManager's storage
	std::shared_ptr<Texture> textureData = std::make_shared<Texture>(textureFilename);

	textures[textureFilename] = textureData;

	return textureData;
}

void ResourceManager::update(float dt)
{
	/*
	//I use a timer here so the code is only run occasionally
	updateDelayTimer.update(dt);
	
	if (updateDelayTimer.hasTimerFinished())
	{
		checkForExpiredResources(models);
		checkForExpiredResources(audio);
		checkForExpiredResources(textures);

		updateDelayTimer.restart();
	}
	*/
}

/*
template<class R>
void ResourceManager::checkForExpiredResources(std::unordered_map<std::string, R> &resourceArray)
{
	//Loop through the resource array
	for (auto it = resourceArray.begin(); it != resourceArray.end(); )
	{
		//If the resource isn't being used and isn't flaged to stay loaded
		if (it->second->getInstanceCount() == 0 &&
			!it->second->keepLoaded())
		{
			//Delete from the resource array
			it = resourceArray.erase(it);
		}
		else 
		{
			it++;
		}
	}
}
*/