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

namespace Setting 
{
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
	
	@param settingsFilePath Relative pathname of the settings file.
	 */
	Platform(std::string settingsFilePath);

	/** @brief Destructor, Calls SDL's cleanup features for itself and its add ons. */
	~Platform();

	/**
	 @brief	Initialises the SDL library and its plugins, for the current platform.
	
	 @param	openGL	   	true to use openGL.
	 @param	windowTitle	The window title.
	
	 @return	bool - Was successful.
	 */

	bool initSDL(bool openGL, std::string windowTitle);

	/** @brief	Loads game's settings from the settings file. */
	void loadSettingsFromFile();


	/**
	 @brief Gets the window.
	
	 @return null if it fails, else the window.
	 */
	SDL_Window* getWindow() { return window; }

	/**
	@brief Gets the GL context.
	
	@return null if it fails, else the context.
	 */
	SDL_GLContext getContext() { return context; }

	/**
	 @brief Gets window size.
	
	 @return The window size.
	 */
	Vec2 getWindowSize() { return windowSize; }


	int getSetting(std::string setting);

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

	//The resolution everything is scaled from
	const Vec2 scale;


	//Settings
	void initSettingsFile();

	bool settingsFileExists();

	const std::string settingsFilePath;

	const std::string defaultSettingsPath;

	Setting::FullscreenMode fullscreenMode;
	
	std::unordered_map<std::string, int> settings;

	//Platform Feature Support (Wraps SDL CPU feature detection)
	std::unordered_map<std::string, bool> features;

	void checkFeatureSupport();

	
};