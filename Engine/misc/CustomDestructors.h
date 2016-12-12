#pragma once

#include <SDL.h>

/** @brief	Contains two SDL variables that can't easily be wrapped in unique_ptrs by themselves. */
struct SDL_Context
{
	SDL_Window* window;
	SDL_GLContext glContext;
};

/** @brief	Contains deletor functions for Unique pointers. */
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