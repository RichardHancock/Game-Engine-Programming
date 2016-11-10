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
std::unordered_map<std::string, GameModel*> ResourceManager::models;
std::unordered_map<std::string, Audio*> ResourceManager::audio;
std::unordered_map<std::string, Texture*> ResourceManager::textures;
Assimp::Importer* ResourceManager::modelImporter = new Assimp::Importer();
Utility::SimpleTimer ResourceManager::updateDelayTimer(UPDATE_DELAY);

std::shared_ptr<EngineState> ResourceManager::engineState;


ResourceManager::ResourceManager() {}

void ResourceManager::cleanUp()
{
	for(auto model : models)
	{
		delete model.second;
	}
	models.clear();

	for (auto curAudio : audio)
	{
		delete curAudio.second;
	}
	audio.clear();

	for (auto texture : textures)
	{
		delete texture.second;
	}
	textures.clear();
	
	delete modelImporter;
}

Audio* ResourceManager::getAudio(std::string audioFilename, bool isMusic, bool defaultPath)
{
	//Should we use the default path for this type of resource or just use the provided filename
	if (defaultPath) {
		audioFilename = audioDir + audioFilename;
	}
	
	//Is it already loaded
	if (audio.count(audioFilename) > 0)
	{
		//Get a instance of the resource and increment its internal counter
		audio[audioFilename]->loadResourceInstance();
		return audio[audioFilename];
	}

	//Else load the resource and add it to the resourceManager's storage
	Audio* audioFile;
			
	if (isMusic)
	{
		audioFile = new Music(audioFilename);
	}
	else
	{
		audioFile = new SFX(audioFilename);
	}

	audio[audioFilename] = audioFile;
	
	return audioFile;
}

GameModel* ResourceManager::getModel(std::string modelFilename, bool defaultPath)
{
	//Should we use the default path for this type of resource or just use the provided filename
	if (defaultPath) {
		modelFilename = modelDir + modelFilename;
	}

	//Is it already loaded
	if (models.count(modelFilename) > 0)
	{
		//Get a instance of the resource and increment its internal counter
		models[modelFilename]->loadResourceInstance();
		return models[modelFilename];
	}

	
	Log::logI("Loading model: " + modelFilename);

	//Read in the model file and apply Post processing flags
	int flags = (
		aiProcess_JoinIdenticalVertices | 
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace
		);
	const aiScene* rawModelData = modelImporter->ReadFile(modelFilename, flags);

	if (rawModelData == nullptr)
	{
		Log::logE(modelFilename + " import failed: " + modelImporter->GetErrorString());
		return nullptr;
	}

	//TEMP get the first mesh only
	aiMesh* mesh = rawModelData->mMeshes[0];


	GameModel* modelData = new GameModel(mesh);
	
	//We've converted the data to our formats so delete the raw version.
	modelImporter->FreeScene();

	models[modelFilename] = modelData;

	return modelData;
}

Texture* ResourceManager::getTexture(std::string textureFilename, bool defaultPath)
{
	//Should we use the default path for this type of resource or just use the provided filename
	if (defaultPath) {
		textureFilename = textureDir + textureFilename;
	}

	//Is it already loaded
	if (textures.count(textureFilename) > 0)
	{
		//Get a instance of the resource and increment its internal counter
		textures[textureFilename]->loadResourceInstance();
		return textures[textureFilename];
	}

	//Else load the resource and add it to the resourceManager's storage
	Texture* textureData = new Texture(textureFilename);

	textures[textureFilename] = textureData;

	return textureData;
}

void ResourceManager::update(float dt)
{
	//I use a timer here so the code is only run occasionally
	updateDelayTimer.update(dt);
	
	if (updateDelayTimer.hasTimerFinished())
	{
		checkForExpiredResources(models);
		checkForExpiredResources(audio);
		checkForExpiredResources(textures);

		updateDelayTimer.restart();
	}
}

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