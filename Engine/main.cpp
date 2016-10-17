#define GLM_FORCE_CXX11

#include <SDL.h>

#include "Platform.h"
#include "misc/Utility.h"
#include "misc/Log.h"
#include "states/StateManager.h"
#include "states/Game.h"
#include "ResourceManager.h"

#ifdef _WIN32
#include <windows.h>

//This forces NVIDIA hybrid GPU's (Intel and Nvidia integrated) to use the high performance NVidia chip rather than the Intel.
//This was recommended by NVidia's policies: http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif


void processCommandLine(int argc, char **argv);
int main(int argc, char *argv[]);


int main(int argc, char *argv[])
{
	processCommandLine(argc, argv);

	std::string settingsFilename = "settings.xml";

	Platform* platform = new Platform(settingsFilename);
	platform->loadSettingsFromFile("RH", "Engine");

	if (!platform->initSDL(true, "Engine"))
	{
		Log::logE("SDL Failed to initialize");
		exit(1);
	}


	glEnable(GL_DEPTH_TEST);
	
	
	if (platform->getSetting("MSAA") != 0)
		glEnable(GL_MULTISAMPLE);
	
	
	Utility::randomInit();


	SDL_Window* window = platform->getWindow();

	StateManager* manager = new StateManager((int)platform->getWindowSize().x, (int)platform->getWindowSize().y);

	manager->addState(new Game(manager, platform));

	unsigned int lastTime = SDL_GetTicks();

	bool done = false;

	while (!done)
	{
		done = manager->eventHandler();

		// Update
		//Calculate deltaTime
		unsigned int current = SDL_GetTicks();
		float dt = (float)(current - lastTime) / 1000.0f;
		lastTime = current;

		Utility::Timer::update(dt);

		manager->update(dt);

		InputManager::update();
		ResourceManager::update(dt);

		//Render
		// Specify the colour to clear the framebuffer to
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		// This writes the above colour to the colour part of the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		manager->render();

		SDL_GL_SwapWindow(window);

		if (dt < (1.0f / 50.0f))
		{
			SDL_Delay((unsigned int)(((1.0f / 50.0f) - dt)*1000.0f));
		}
	}

	delete manager;
	InputManager::cleanup();
	ResourceManager::cleanUp();
	delete platform;
	SDL_Quit();
	
	exit(0);
}

void processCommandLine(int argc, char **argv)
{
	std::cout << "No command line arguments are handled currently" << std::endl;
	std::cout << "These were the passed in args:" << std::endl;
	for (int i = 1; i < argc; i++)
	{
		std::cout << argv[i];
	}
}