#include "Platform.h"

#include <string>
#include <fstream>

#include "misc/Log.h"
#include "misc/Utility.h"

std::unique_ptr<SDL_Context, CustomDestructors::SDL_Deleter> Platform::sdlContext;
Vec2 Platform::windowSize = Vec2(0.0f);
std::string Platform::settingsFilename;
std::string Platform::settingsFilePath;
const std::string Platform::defaultSettingsPath = "resources/defaultSettings.xml";
Setting::FullscreenMode Platform::fullscreenMode = Setting::FullscreenMode::Windowed;
std::unordered_map<std::string, int> Platform::settings;
std::unordered_map<std::string, bool> Platform::features;
Platform::Renderer Platform::currentRenderer;


void Platform::init(std::string newSettingsFilename)
{
	Platform::settingsFilename = newSettingsFilename;

	sdlContext = std::unique_ptr<SDL_Context, CustomDestructors::SDL_Deleter>(new SDL_Context());
	sdlContext->glContext = nullptr;
	sdlContext->window = nullptr;
}

void Platform::cleanup()
{
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	
	sdlContext.reset();
}

bool Platform::initGLEW()
{
	// GLEW has a problem with loading core OpenGL
	// See here: https://www.opengl.org/wiki/OpenGL_Loading_Library
	// The temporary workaround is to enable its 'experimental' features
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		Log::logE("GLEW failed to initialise with message : " + std::string((char*) glewGetErrorString(err))); 
		return false;
	}
	Log::logI("Using GLEW " + std::string((char*)glewGetString(GLEW_VERSION)));

	Log::logI("OpenGL Vendor: " + std::string((char*)glGetString(GL_VENDOR)));
	Log::logI("OpenGL Renderer: " + std::string((char*)glGetString(GL_RENDERER)));
	Log::logI("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)));
	Log::logI("OpenGL Shading Language Version: " + std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));

	return true;
}


bool Platform::initSDL(bool openGL, std::string windowTitle)
{
	///@todo abort program on every error rather than just waiting

	bool status = true;
	
	Log::logI("Initializing SDL and its plugins.");

	int sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC;
	int sdlResult = SDL_Init(sdlFlags); //Android doesn't need this at SDL internally does this
	
	if (sdlResult != 0)
	{ 
		status = false; 
		Log::logE("SDL Init failed: " + std::string(SDL_GetError()));
	}
	
	Log::logI("SDL Version: Compiled: " + getSDLVersionString(false) + 
		" / Linked: " + getSDLVersionString(true));

	//SDL TTF Initialization
	if (TTF_Init() == -1)
	{
		status = false;
		Log::logE("SDL_ttf init failed: " + std::string(TTF_GetError()));
	}

	Log::logI("SDL_ttf Version: Compiled: " + getSDLTtfVersionString(false) +
		" / Linked: " + getSDLTtfVersionString(true));

	//SDL Mixer Initialization
	int mixFlags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC;
	int mixResult = Mix_Init(mixFlags);

	//  If the inputed flags are not returned, an error has occurred
	if ((mixResult & mixFlags) != mixFlags)
	{
		status = false;
		Log::logE("SDL_mixer init failed (or a requested format was not available): " + std::string(Mix_GetError()));
	}
	
	//Initialize SDL_Mixer with some standard audio formats/freqs. Also set channels to 2 for stereo sound.
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		status = false;
		Log::logE("SDL_mixer could not open the audio device: " + std::string(Mix_GetError()));
	}

	Log::logI("SDL_mixer Version: Compiled: " + getSDLMixerVersionString(false) +
		" / Linked: " + getSDLMixerVersionString(true));

	//SDL Image Initialization
	int imgFlags = IMG_INIT_PNG |IMG_INIT_JPG | IMG_INIT_TIF;
	int imgResult = IMG_Init(imgFlags);
	
	//  If the inputed flags are not returned, an error has occurred
	if((imgResult & imgFlags) != imgFlags) 
	{
		Log::logE("SDL_image init failed (or a requested format was not available): "+ std::string(IMG_GetError()));
	}

	Log::logI("SDL_image Version: Compiled: " + getSDLImageVersionString(false) +
		" / Linked: " + getSDLImageVersionString(true));


	//Set OpenGL params
	///@todo Make these configurable
	if (openGL)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		if (settings["MSAA"] != 0)
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, settings["MSAA"]);
		}
	}
	
	
	uint32_t windowFlags = SDL_WINDOW_OPENGL; 

	switch (fullscreenMode)
	{
	case Setting::Windowed:
		Log::logI("Window Mode: Windowed");
		break;

	case Setting::Fullscreen:
		Log::logI("Window Mode: Fullscreen");
		windowFlags |= SDL_WINDOW_FULLSCREEN;
		break;
	
	case Setting::Borderless:
		Log::logI("Window Mode: Borderless");
		windowFlags |= SDL_WINDOW_BORDERLESS;
		break;
	}

	sdlContext->window = SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		(int) windowSize.x, 
		(int) windowSize.y,
		windowFlags
	);


	if (sdlContext->window == nullptr) 
	{ 
		status = false;
		Log::logE("Window failed to be created: " + 
			std::string(SDL_GetError()));
	}
	
	currentRenderer = Renderer::Dummy;

	if (openGL) 
	{
		sdlContext->glContext = SDL_GL_CreateContext(sdlContext->window);

		if (sdlContext->glContext == nullptr) 
		{
			#ifdef NDEBUG
			return false;
			#endif // NDEBUG

			Log::logE("GL context failed to be created: " + std::string(SDL_GetError()));
			Log::logI("Dummy Renderer will be used instead. THIS PROVIDES NO DISPLAY OUTPUT");
			currentRenderer = Renderer::Dummy;
		}
		else
		{
			if (!initGLEW())
			{
				status = false;
			}
			currentRenderer = Renderer::OpenGL;
		}
	}

	int width;
	int height;

	SDL_GetWindowSize(sdlContext->window, &width, &height);

	windowSize.x = (float)width;
	windowSize.y = (float)height;
	
	Log::logI("Window Dimensions: " + Utility::intToString(width) +
		"x" + Utility::intToString(height));

	//Feature Detection
	checkFeatureSupport();

	//Platform Details
	Log::logI("CPU Cores: " + Utility::intToString(SDL_GetCPUCount()));
	Log::logI("CPU L1 Cache: " + Utility::intToString(SDL_GetCPUCacheLineSize()) + "KB");
	Log::logI("RAM: " + Utility::intToString(SDL_GetSystemRAM()) + "MB");


	return status;
}


std::string Platform::versionStringConverter(SDL_version * version)
{
	return (Utility::intToString(version->major) + "." + 
		Utility::intToString(version->minor) + "." + 
		Utility::intToString(version->patch));
}

std::string Platform::getSDLVersionString(bool linked)
{
	SDL_version* version = new SDL_version();

	if (linked)
	{
		SDL_GetVersion(version);
	}
	else
	{
		SDL_VERSION(version);
	}

	std::string result = versionStringConverter(version);
	SDL_free(version);
	return result;
}

std::string Platform::getSDLImageVersionString(bool linked)
{
	SDL_version* version = new SDL_version();

	if (linked)
	{
		//Small hack to stop having to do bad const casts
		const SDL_version* tempVersion = IMG_Linked_Version();
		version->major = tempVersion->major;
		version->minor = tempVersion->minor;
		version->patch = tempVersion->patch;
		//SDL_free(tempVersion); I don't know if this is necessary.
	}
	else
	{
		SDL_IMAGE_VERSION(version);
	}

	std::string result = versionStringConverter(version);
	SDL_free(version);
	return result;
}

std::string Platform::getSDLMixerVersionString(bool linked)
{
	SDL_version* version = new SDL_version();

	if (linked)
	{
		//Small hack to stop having to do bad const casts
		const SDL_version* tempVersion = Mix_Linked_Version();
		version->major = tempVersion->major;
		version->minor = tempVersion->minor;
		version->patch = tempVersion->patch;
		//SDL_free(tempVersion); I don't know if this is necessary.
	}
	else
	{
		SDL_MIXER_VERSION(version);
	}

	std::string result = versionStringConverter(version);
	SDL_free(version);
	return result;
}

std::string Platform::getSDLTtfVersionString(bool linked)
{
	SDL_version* version = new SDL_version();

	if (linked)
	{
		//Small hack to stop having to do bad const casts
		const SDL_version* tempVersion = TTF_Linked_Version();
		version->major = tempVersion->major;
		version->minor = tempVersion->minor;
		version->patch = tempVersion->patch;
		//SDL_free(tempVersion); I don't know if this is necessary.
	}
	else
	{
		SDL_TTF_VERSION(version);
	}

	std::string result = versionStringConverter(version);
	SDL_free(version);
	return result;
}

void Platform::loadSettingsFromFile(std::string org, std::string app)
{
	//Get the path and combine with filename for settings file location
	char* basePath = SDL_GetPrefPath(org.c_str(), app.c_str());
	settingsFilePath = basePath + settingsFilename;
	SDL_free(basePath);

	Log::logI("Loading Settings from: " + settingsFilePath);

	//Check if file exists, if not create the base settings file
	if (!settingsFileExists())
	{
		initSettingsFile();
	}

	pugi::xml_document settingsFile;
	pugi::xml_parse_result result = settingsFile.load_file(settingsFilePath.c_str());

	if (!result)
	{
		Log::logE("Settings File could not be parsed:");
		Log::logE(" - Path: " + settingsFilePath);
		Log::logE(" - Error Description: " + std::string(result.description()));

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Settings File Missing or Corrupted",
			"Settings File Missing or Corrupted",
			NULL);

		exit(-1);
	} 

	//TODO make this less repetitive.
	//TODO need to check if the setting is actually being fetched or whether the default value is used instead (0).

	//Resolution
	pugi::xml_node resolution = settingsFile.child("resolution");
	Vec2 tempWindowSize = {
		(float) atoi(resolution.attribute("width").value()),
		(float) atoi(resolution.attribute("height").value())
	};
	Log::logI("Window Size from Settings File: " + Utility::vec2ToString(tempWindowSize));

	if (tempWindowSize.x < 640 || tempWindowSize.y < 480)
	{
		Log::logE("Windows size is too small, please increase (min 640x480) in settings file");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Resolution Invalid",
			"Resolution in settings file is too small, please increase to at least 640x480.",
			NULL);
		exit(-1);
	}

	windowSize = tempWindowSize;


	//Fullscreen
	int tempFullscreenMode = atoi(settingsFile.child("fullscreen").child_value());
	Log::logI("Fullscreen Mode from Settings File: " + Utility::intToString(tempFullscreenMode));

	if (tempFullscreenMode < 0 || tempFullscreenMode > 2)
	{
		Log::logE("Fullscreen Mode Setting is invalid, please reset to 0, 1 or 2 in settings file");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Fullscreen Mode Invalid",
			"Fullscreen Mode Setting is invalid, please reset to 0, 1 or 2 in settings file.",
			NULL);
		exit(-1);
	}

	fullscreenMode = (Setting::FullscreenMode) tempFullscreenMode;

	
	//MSAA
	int msaaSamples = atoi(settingsFile.child("MSAA").child_value());
	Log::logI("Anti Aliasing sample count from Settings File: " + Utility::intToString(msaaSamples));

	if (msaaSamples < 0 || msaaSamples > 16)
	{
		Log::logE("MSAA Setting is invalid, please reset to 0 - 16 in settings file");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"MSAA Setting Invalid",
			"MSAA Setting is invalid, please reset to 0 - 16 in settings file.",
			NULL);
		exit(-1);
	}

	settings["MSAA"] = msaaSamples;
	
}

Platform::Renderer Platform::getCurrentRenderer()
{
	return currentRenderer;
}

bool Platform::isDummyRenderer()
{
	return currentRenderer == Renderer::Dummy;
}

void Platform::initSettingsFile()
{
	std::ifstream defaultSettings(defaultSettingsPath);

	if (!defaultSettings.is_open())
	{
		Log::logE("Default Settings file missing");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Default Settings file missing",
			"Default Settings file missing. Please reinstall the program.",
			NULL);
		exit(-1);
	}


	Log::logI("Creating Settings File at " + settingsFilePath);
	std::ofstream newSettingsFile(settingsFilePath, std::fstream::trunc);

	if (!newSettingsFile.is_open())
	{
		Log::logE("Could not create Settings file");
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Could not create Settings file",
			"Could not create Settings file. This could be a permissions issue.",
			NULL);
		exit(-1);
	}


	while (!defaultSettings.eof())
	{
		char line[1024];

		defaultSettings.getline(line, 1024);
		newSettingsFile << line << std::endl;
	}
}

bool Platform::settingsFileExists()
{
	std::ifstream settingsFile(settingsFilePath);
	
	return settingsFile.is_open();
}


void Platform::sdlSwapWindow()
{
	if (currentRenderer != Renderer::Dummy)
	{
		SDL_GL_SwapWindow(sdlContext->window);
	}
}

int Platform::getSetting(std::string setting)
{
	if (settings.count(setting) == 0)
	{
		Log::logW(setting + " setting does not exist, returning 0.");
		return 0;
	}

	return settings[setting];
}


bool Platform::isFeatureSupported(std::string feature)
{
	if (features.count(feature) == 0)
	{
		Log::logW(feature + " feature is unknown, assuming unsupported.");
		return false;
	}

	return features[feature];
}

void Platform::checkFeatureSupport()
{
	features["3DNow"] = SDL_Has3DNow() == SDL_TRUE;
	features["AVX"] = SDL_HasAVX() == SDL_TRUE;
	features["AVX2"] = SDL_HasAVX2() == SDL_TRUE;
	features["AltiVec"] = SDL_HasAltiVec() == SDL_TRUE;
	features["MMX"] = SDL_HasMMX() == SDL_TRUE;
	features["RDTSC"] = SDL_HasRDTSC() == SDL_TRUE;
	features["SSE"] = SDL_HasSSE() == SDL_TRUE;
	features["SSE2"] = SDL_HasSSE2() == SDL_TRUE;
	features["SSE3"] = SDL_HasSSE3() == SDL_TRUE;
	features["SSE41"] = SDL_HasSSE41() == SDL_TRUE;
	features["SSE42"] = SDL_HasSSE42() == SDL_TRUE;

	Log::logI("Platform Features:");
	for (auto feature : features)
	{
		Log::logI(" - " + feature.first + ": " + (feature.second ? "Yes" : "No"));
	}
}