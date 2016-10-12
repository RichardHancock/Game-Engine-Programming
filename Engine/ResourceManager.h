#pragma once

#include <string>
#include <unordered_map>

#include <assimp/Importer.hpp>

#include "GameModel.h"
#include "Audio.h"
#include "Texture.h"
#include "Resource.h"
#include "misc/Utility.h"

// Forward Declaration
class GameModel;


/**
@brief Manager for resources of most types.

Loads and manages any resources.
*/
class ResourceManager
{
public:
	
	ResourceManager();

	~ResourceManager();

	/**
	@brief Gets a audio object, loads the audio if not already loaded.
	
	@param audioFilename Filename of the audio file.
	@param isMusic		 true if this audio is music, rather than a sfx.
	@param defaultPath   true to use default audio path.
	
	@return null if it fails, else the audio.
	 */
	Audio* getAudio(std::string audioFilename, bool isMusic, bool defaultPath = true);

	/**
	@brief Gets a model, loads the model if not already loaded.
	
	@param modelFilename    Filename of the model file.
	@param [in,out] texture Standard Texture.
	@param defaultPath	    true to use default model path.
	
	@return null if it fails, else the model.
	
	@todo Support multi mesh models.
	 */
	GameModel* getModel(std::string modelFilename, Texture* texture, bool defaultPath = true);

	/**
	@brief Gets a texture, loads the texture if not already loaded.
	
	@param textureFilename Filename of the texture file.
	@param defaultPath	   true to use default texture path.
	
	@return null if it fails, else the texture.
	 */
	Texture* getTexture(std::string textureFilename, bool defaultPath = true);


	/**
	@brief Goes through all the assets and checks if any need deleting.
	
	@param dt DeltaTime.
	
	@todo implement some way of limiting this, as it's not needed every frame.
	 */
	void update(float dt);

	/** @brief Values that represent resource types. */
	enum ResourceTypes
	{
		ModelFile,
		AudioFile,
		TextureFile
	};

	/** @brief The directory for the relevant asset type */
	const std::string modelDir, shaderDir, audioDir, textureDir;

private:
	/** @brief The models. */
	std::unordered_map<std::string, GameModel*> models;

	/** @brief The audio. */
	std::unordered_map<std::string, Audio*> audio;

	/** @brief The textures. */
	std::unordered_map<std::string, Texture*> textures;

	
	/** @brief The model importer from Assimp. */
	Assimp::Importer* modelImporter;


	/** @brief Only check the internal resources every 10 seconds. */
	const float UPDATE_DELAY = 10.0f;
	
	/** @brief The update delay timer to stop update running every frame. */
	Utility::SimpleTimer updateDelayTimer;
	

	
	/**
	@brief Checks for expired resources and deletes any not being used.
	
	@param [in,out] resourceArray Array of resources.
	 */
	template<class R>
	void checkForExpiredResources(std::unordered_map<std::string, R> &resourceArray);
};