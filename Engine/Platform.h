#pragma once

#include <unordered_map>
#include <pugixml.hpp>
#include <GL/glew.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "misc/Vec2.h"

//Quick Pre-processor key 
#ifdef _WIN32
	//Windows 32/64 bit
#elif __ANDROID__
	//Android (Is also covered by __linux__ preprocessor so must be before it)
#elif EMSCRIPTEN
	//Emscripten (Web Browsers), not sure if this is correct macro
	// Cant develop for Emscripten as it lacks support for a few major SDL 2 add ons.
#elif __APPLE__
	//Mac OSX and iOS (Probably wont be used as I can't test either)
#elif __linux__
	//Linux Kernel
#endif

/** @brief	Contains Setting enums. */
namespace Setting 
{
	/** @brief	Enums that represent different fullscreen modes. */
	enum FullscreenMode
	{
		Windowed = 0,
		Fullscreen = 1,
		Borderless = 2
	};
}


/** @brief Class that handles the initialization of SDL 2 (and its add ons) across all supported platforms */
class Platform
{
public:

	/**
	@brief Constructor.
	
	@param settingsFilename Exact filename of the settings file (Not including Path).
	 */
	Platform(std::string settingsFilename);

	/** @brief Destructor, Calls SDL's cleanup features for itself and its add ons. */
	~Platform();

	/**
	 @brief	Initialises the SDL library and its plugins, for the current platform.
	
	 @param	openGL	   	true to use openGL.
	 @param	windowTitle	The window title.
	
	 @return	true if it succeeds, false if it fails.
	 */
	bool initSDL(bool openGL, std::string windowTitle);

	/**
	 @brief	Loads game's settings from the settings file.
	
	 @param	org	The organisation name, will be the root directory of the setting file.
	 @param	app	The application name, will be a subfolder in the above directory.
	 */
	void loadSettingsFromFile(std::string org, std::string app);


	/**
	 @brief Gets the SDL window.
	
	 @return null if it fails, else the window.
	 */
	SDL_Window* getWindow() { return window; }

	/**
	@brief Gets the GL context.
	
	@return null if it fails, else the context.
	 */
	SDL_GLContext getContext() { return context; }

	/**
	 @brief Gets a Vec2 containing the windows size.
	
	 @return The window size.
	 */
	Vec2 getWindowSize() { return windowSize; }

	/**
	 @brief	Gets a specified setting value.
	
	 @param	setting	The setting's name/identifier.
	
	 @return	The setting's value.
	 */
	int getSetting(std::string setting);

	/**
	 @brief	Query if 'feature' is supported using SDLs CPU feature checks.
	
	 @param	feature	The feature.
	
	 @return	true if feature supported, false if not.
	 */
	bool isFeatureSupported(std::string feature);
	
private:

	/**
	@brief Initialises glew.

	@return true if it succeeds, false if it fails.
	*/
	bool initGLEW();

	/** @brief The window. */
	SDL_Window* window;

	/** @brief The GL context. */
	SDL_GLContext context;

	/** @brief Size of the window. */
	Vec2 windowSize;

	/** @brief The resolution everything is scaled from */
	const Vec2 scale;


	//Settings

	/** @brief	Create a local settings file from the default settings file. */
	void initSettingsFile();

	/**
	 @brief	Checks if a local settings file already exists.
	
	 @return	true if it exist, false if it does not.
	 */
	bool settingsFileExists();

	/** @brief	Filename of the settings file. */
	const std::string settingsFilename;

	/** @brief	Full pathname of the settings file. */
	std::string settingsFilePath;

	/** @brief	The path of the default settings file in the resource folder. */
	const std::string defaultSettingsPath;

	/** @brief	The current fullscreen mode specified by the settings file. */
	Setting::FullscreenMode fullscreenMode;
	
	/** @brief	Array containing all the setting data */
	std::unordered_map<std::string, int> settings;


	//Platform Feature Support (Wraps SDL CPU feature detection)
	
	/** @brief	Array containing all the system features and their availability */
	std::unordered_map<std::string, bool> features;

	/** @brief	Populate the features array by checking all of SDL's feature functions */
	void checkFeatureSupport();

	
};