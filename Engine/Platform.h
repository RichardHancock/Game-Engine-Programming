#pragma once

#include <unordered_map>
#include <pugixml.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <memory>

#include "misc/Vec2.h"
#include "misc/CustomDestructors.h"

//Ref: Was used in previous assignment but with massive changes

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

	enum Renderer
	{
		Dummy,
		OpenGL
		//OpenGL_Legacy
	};

	/**
	@brief Constructor.
	
	@param newSettingsFilename Exact filename of the settings file (Not including Path).
	 */
	static void init(std::string newSettingsFilename);

	/** @brief Destructor, Calls SDL's cleanup features for itself and its add ons. */
	static void cleanup();

	/**
	 @brief	Initialises the SDL library and its plugins, for the current platform.
	
	 @param	openGL	   	true to use openGL.
	 @param	windowTitle	The window title.
	
	 @return	true if it succeeds, false if it fails.
	 */
	static bool initSDL(bool openGL, std::string windowTitle);

	/**
	 @brief	Loads game's settings from the settings file.
	
	 @param	org	The organisation name, will be the root directory of the setting file.
	 @param	app	The application name, will be a subfolder in the above directory.
	 @param	resourceDir	The resource directory that contains the default settings file (e.x "/usr/local/share/" do not include "/resources" as this is assumed).
	 */
	static void loadSettingsFromFile(std::string org, std::string app, std::string resourceDir);

	/**
	 @brief	Gets current renderer.
	
	 @return	The current renderer.
	 */
	static Renderer getCurrentRenderer();

	/**
	 @brief	Query if the current renderer is dummy renderer.
	
	 @return	True if dummy renderer, false if not.
	 */
	static bool isDummyRenderer();

	/**
	 @brief Gets a Vec2 containing the windows size.
	
	 @return The window size.
	 */
	static Vec2 getWindowSize() { return windowSize; }

	/** @brief	Swap buffer for OpenGL double buffering. */
	static void sdlSwapWindow();


	/**
	 @brief	Gets a specified setting value.
	
	 @param	setting	The setting's name/identifier.
	
	 @return	The setting's value.
	 */
	static int getSetting(std::string setting);

	/**
	 @brief	Query if 'feature' is supported using SDLs CPU feature checks.
	
	 @param	feature	The feature.
	
	 @return	true if feature supported, false if not.
	 */
	static bool isFeatureSupported(std::string feature);
	
private:

	/**
	@brief Initialises GLEW.

	@return true if it succeeds, false if it fails.
	*/
	static bool initGLEW();

	/** @brief The window and GL Context. */
	static std::unique_ptr<SDL_Context, CustomDestructors::SDL_Deleter> sdlContext;

	/** @brief Size of the window. */
	static Vec2 windowSize;

	/** @brief	The current renderer. */
	static Renderer currentRenderer;
	
	//Version Number
	/**
	 @brief	SDL Version number to string converter.
	
	 @param [in,out]	version	If non-null, the version.
	
	 @return	A std::string.
	 */
	static std::string versionStringConverter(SDL_version* version);

	/** 
	@brief Gets a version string of SDL
	Using the parameter you can compare the runtime version to the version that you compiled with.

	@param linked Return the linked version number, or the compiled version number.
	@return Version String in this format "0.0.0" or "major.minor.patch".
	*/
	static std::string getSDLVersionString(bool linked);

	/**
	@brief Gets a version string of SDL_image
	Using the parameter you can compare the runtime version to the version that you compiled with.

	@param linked Return the linked version number, or the compiled version number.
	@return Version String in this format "0.0.0" or "major.minor.patch".
	*/
	static std::string getSDLImageVersionString(bool linked);

	/**
	@brief Gets a version string of SDL_mixer
	Using the parameter you can compare the runtime version to the version that you compiled with.

	@param linked Return the linked version number, or the compiled version number.
	@return Version String in this format "0.0.0" or "major.minor.patch".
	*/
	static std::string getSDLMixerVersionString(bool linked);

	/**
	@brief Gets a version string of SDL_ttf
	Using the parameter you can compare the runtime version to the version that you compiled with.

	@param linked Return the linked version number, or the compiled version number.
	@return Version String in this format "0.0.0" or "major.minor.patch".
	*/
	static std::string getSDLTtfVersionString(bool linked);

	//Settings

	/**
	 @brief	Create a local settings file from the default settings file.
	
	 @param	resourceDir	The resource directory that contains the default settings file (e.x "/usr/local/share/" do not include "/resources" as this is assumed).
	 */
	static void initSettingsFile(std::string resourceDir);

	/**
	 @brief	Checks if a local settings file already exists.
	
	 @return	true if it exist, false if it does not.
	 */
	static bool settingsFileExists();

	/** @brief	Filename of the settings file. */
	static std::string settingsFilename;

	/** @brief	Full pathname of the settings file. */
	static std::string settingsFilePath;

	/** @brief	The path of the default settings file in the resource folder. */
	static const std::string defaultSettingsPath;

	/** @brief	The current fullscreen mode specified by the settings file. */
	static Setting::FullscreenMode fullscreenMode;
	
	/** @brief	Array containing all the setting data */
	static std::unordered_map<std::string, int> settings;


	//Platform Feature Support (Wraps SDL CPU feature detection)
	
	/** @brief	Array containing all the system features and their availability */
	static std::unordered_map<std::string, bool> features;

	/** @brief	Populate the features array by checking all of SDL's feature functions */
	static void checkFeatureSupport();

};