#define GLM_FORCE_CXX11

#include <SDL.h>
#include <assimp/version.h>
#include <GL/glew.h>

#include "Platform.h"
#include "misc/Utility.h"
#include "misc/Log.h"
#include "states/StateManager.h"
#include "states/Game.h"
#include "ResourceManager.h"
#include "misc/Random.h"
#include "misc/DeltaTime.h"
#include "Physics.h"
#include "misc/debugDrawer/DebugDrawer.h"

#ifdef _WIN32
#include <windows.h>

//This forces NVIDIA hybrid GPU's (Intel and NVidia integrated) to use the high performance NVidia chip rather than the Intel.
//This was recommended by NVidia's policies: http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif


void processCommandLine(int argc, char **argv);
int main(int argc, char *argv[]);


int main(int argc, char *argv[])
{
	std::string organisation = "RH";
	std::string application = "Engine";

	std::string resourceDir;

#ifdef __APPLE__ //Workaround for mac as it doesn't like relative directories (Probably Linux would also benefit from this change).
	resourceDir = "/usr/local/share/" + organisation + "/" + application + "/";
	ResourceManager::setResourceDirPath(resourceDir);
#endif

	//Init Log Subsystem first as everything uses it.
	Log::init(true, organisation, application);

	processCommandLine(argc, argv);

	std::string settingsFilename = "settings.xml";

	Platform::init(settingsFilename);
	Platform::loadSettingsFromFile(organisation, application, resourceDir);
	Log::logI("Settings fully loaded.");

	if (!Platform::initSDL(true, "Engine"))
	{
		Log::logE("SDL Failed to initialize");
		exit(1);
	}


	glEnable(GL_DEPTH_TEST);


	if (Platform::getSetting("MSAA") != 0)
		glEnable(GL_MULTISAMPLE);

	Log::logI("Assimp Version: " + Utility::intToString(aiGetVersionMajor()) +
		"." + Utility::intToString(aiGetVersionMinor())
		+ "." + Utility::intToString(aiGetVersionRevision()));

	Random::init();
	DebugDrawer::init("debugV.glsl", "debugF.glsl");
	Physics::init();

	StateManager::addState(std::make_shared<Game>());

	DeltaTime::init();

	bool done = false;
	bool renderDebugWorld = false;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); Really funky looking toggle, Maybe add to a render manager.
	while (!done)
	{
		done = StateManager::eventHandler();

		if (InputManager::wasKeyReleased(SDLK_F1))
		{
			renderDebugWorld = !renderDebugWorld;
		}

		// Update
		//Calculate deltaTime
		DeltaTime::update();


		Utility::Timer::update();

		StateManager::update();

		Physics::getWorld()->stepSimulation(DeltaTime::getDT(), 10);
		//ResourceManager::update(dt);

		//Pre Render
		if (renderDebugWorld)
		{
			Physics::getWorld()->debugDrawWorld(); //This sets up openGL buffers for rendering
			DebugDrawer::preRender();
		}

		//Render
		if (!Platform::isDummyRenderer())
		{
			// Specify the colour to clear the frame buffer to
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			// This writes the above colour to the colour part of the frame buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		

		StateManager::render();

		if (renderDebugWorld)
			DebugDrawer::render();

		Platform::sdlSwapWindow();

		//Post Render
		DebugDrawer::postRender();

		InputManager::update();

		/* TODO: This makes physics out of sync, Really need to fix the time-step and main program flow in a future update
		if (DeltaTime::getDT() < (1.0f / 50.0f))
		{
			SDL_Delay((unsigned int)(((1.0f / 50.0f) - DeltaTime::getDT())*1000.0f));
		}*/
	}

	StateManager::cleanup();
	DebugDrawer::cleanup();
	InputManager::cleanup();
	ResourceManager::cleanUp();
	Physics::cleanup();
	Log::cleanup();
	Platform::cleanup();

	SDL_Quit();

	exit(0);
}

void processCommandLine(int argc, char **argv)
{
	Log::logI("No command line arguments are handled currently");
	Log::logI("These were the passed in args:");
	for (int i = 0; i < argc; i++)
	{
		Log::logI(" - " + std::string(argv[i]));
	}
}
