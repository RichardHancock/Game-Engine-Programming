#pragma once

#include <string>
#include <unordered_map>

#include <assimp/Importer.hpp>

#include "GameModel.h"
#include "Audio.h"
#include "Texture.h"
#include "Resource.h"
#include "misc/Utility.h"
#include "Material.h"

// Forward Declaration
class GameModel;

/**
@brief Manager for resources of most types.
@todo Update comments as ptr converted to smart
Loads and manages any resources.
*/
class ResourceManager
{
public:

	/** @brief	Clean up memory used by Resource Manager */
	static void cleanUp();

	/**
	@brief Gets a audio object, loads the audio if not already loaded.
	
	@param audioFilename Filename of the audio file.
	@param isMusic		 true if this audio is music, rather than a sfx.
	@param defaultPath   true to use default audio path.
	
	@return null if it fails, else the audio.
	 */
	static std::weak_ptr<Audio> getAudio(std::string audioFilename, bool isMusic, bool defaultPath = true);

	static void loadMaterials(std::string materialName, const aiScene* scene);

	static std::weak_ptr<Material> getMaterial(std::string materialName, unsigned int index);

	static std::vector<std::weak_ptr<Material>> getMaterials(std::string materialName);

	/**
	@brief Gets a model, loads the model if not already loaded.
	
	@param modelFilename    Filename of the model file.
	@param defaultPath	    true to use default model path.
	
	@return null if it fails, else the model.
	
	@todo Support multi mesh models.
	 */
	static std::weak_ptr<GameModel> getModel(std::string modelFilename, bool defaultPath = true);

	/**
	@brief Gets a texture, loads the texture if not already loaded.
	
	@param textureFilename Filename of the texture file.
	@param defaultPath	   true to use default texture path.
	
	@return null if it fails, else the texture.
	 */
	static std::weak_ptr<Texture> getTexture(std::string textureFilename, bool defaultPath = true);


	/**
	@brief Goes through all the assets and checks if any need deleting.
	
	@param dt DeltaTime.
	
	@todo implement some way of limiting this, as it's not needed every frame.
	 */
	static void update(float dt);

	/** @brief Values that represent resource types. */
	enum ResourceTypes
	{
		ModelFile,
		AudioFile,
		TextureFile,
		MaterialType
	};

	/** @brief The directory for the relevant asset type */
	static const std::string modelDir, shaderDir, audioDir, textureDir;

private:
	ResourceManager();
	
	/** @brief The models. */
	static std::unordered_map<std::string, std::shared_ptr<GameModel>> models;

	static std::unordered_map<std::string, std::vector<std::shared_ptr<Material>>> materials;

	/** @brief The audio. */
	static std::unordered_map<std::string, std::shared_ptr<Audio>> audio;

	/** @brief The textures. */
	static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

	
	/** @brief The model importer from Assimp. */
	static Assimp::Importer* modelImporter;


	/** @brief Only check the internal resources every 10 seconds. */
	static const float UPDATE_DELAY;
	
	/** @brief The update delay timer to stop update running every frame. */
	static Utility::SimpleTimer updateDelayTimer;
	

	
	/**
	@brief Checks for expired resources and deletes any not being used.
	
	@param [in,out] resourceArray Array of resources.
	 */
	//template<class R>
	//static void checkForExpiredResources(std::unordered_map<std::string, R> &resourceArray);
};