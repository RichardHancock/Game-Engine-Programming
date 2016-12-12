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

//Ref: Was used in previous assignment but with massive changes

// Forward Declaration
class GameModel;

/**
@brief Manager for resources of most types.
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

	/**
	 @brief	Loads materials from assimp.
	
	 @param	materialName	Name of the material set.
	 @param	scene			The Assimp scene.
	 */
	static void loadMaterialsFromAssimp(std::string materialName, const aiScene* scene);

	/**
	 @brief	Creates a material manually.
	
	 @param	materialName	  	Name of the material.
	 @param	texture			  	The texture.
	 @param	vertShaderFilename	Filename of the vertical shader file.
	 @param	fragShaderFilename	Filename of the fragment shader file.
	 */
	static void createMaterial(std::string materialName, std::weak_ptr<Texture> texture, 
		std::string vertShaderFilename, std::string fragShaderFilename);

	/**
	 @brief	Gets the material associated with the passed in name at the index location or the first if not specified.
	
	 @param	materialName	Name of the material set.
	 @param	index			(Optional) zero-based index of the material.
	 @param	defaultPath 	(Optional) true to default path.
	
	 @return	The material.
	 */
	static std::weak_ptr<Material> getMaterial(std::string materialName, unsigned int index = 0, bool defaultPath = true);

	/**
	 @brief	Gets the material set associated with the passed in name.
	
	 @param	materialName	Name of the material set.
	 @param	defaultPath 	(Optional) true to use default path.
	
	 @return	The materials.
	 */
	static std::vector<std::weak_ptr<Material>> getMaterials(std::string materialName, bool defaultPath = true);

	/**
	 @brief	Gets a model, loads the model if not already loaded.
	
	 @param	modelFilename	Filename of the model file.
	 @param	useAssimp	 	(Optional) true to use assimp for loading, else uses Legacy OBJ loader.
	 @param	defaultPath  	(Optional) true to use default model path.
	
	 @return	null if it fails, else the model.
	 */
	static std::weak_ptr<GameModel> getModel(std::string modelFilename, bool useAssimp = true, bool defaultPath = true);

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
	//static void update(float dt);

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

	/** @brief	The materials. */
	static std::unordered_map<std::string, std::vector<std::shared_ptr<Material>>> materials;

	/** @brief The audio. */
	static std::unordered_map<std::string, std::shared_ptr<Audio>> audio;

	/** @brief The textures. */
	static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

	/**
	 @brief	Loads model from assimp.
	
	 @param	modelFilename	Filename of the model file.
	
	 @return	The model from assimp.
	 */
	static std::weak_ptr<GameModel> loadModelFromAssimp(std::string modelFilename);

	/**
	 @brief	Loads model with object loader.
	
	 @param	modelFilename	Filename of the model file.
	
	 @return	The model.
	 */
	static std::weak_ptr<GameModel> loadModelWithOBJLoader(std::string modelFilename);

	/** @brief The model importer from Assimp. */
	static std::shared_ptr<Assimp::Importer> modelImporter;


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