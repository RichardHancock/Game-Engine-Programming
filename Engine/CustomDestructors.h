#pragma once

#include <SDL.h>

namespace CustomDestructors
{
	void DeleteSDL_Haptic(SDL_Haptic* haptic);
	
	void DeleteSDL_GameController(SDL_GameController* gamepad);

	void DeleteSDL_Window(SDL_Window* window);

	void DeleteSDL_Surface(SDL_Surface* surface);
}