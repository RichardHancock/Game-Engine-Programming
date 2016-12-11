#pragma once

#include <SDL.h>

struct SDL_Context
{
	SDL_Window* window;
	SDL_GLContext glContext;
};

namespace CustomDestructors
{
	struct SDL_Deleter
	{
		void operator()(SDL_GameController* gamepad);
		void operator()(SDL_Haptic* haptic);
		void operator()(SDL_Window* window);
		void operator()(SDL_GLContext context);
		void operator()(SDL_Context* context);
	};
	
}