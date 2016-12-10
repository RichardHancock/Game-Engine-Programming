#include "CustomDestructors.h"

#include "misc/Log.h"

void CustomDestructors::DeleteSDL_Haptic(SDL_Haptic * haptic)
{
	SDL_HapticClose(haptic);
	Log::logI("Game Controller haptic closed");
}

void CustomDestructors::DeleteSDL_GameController(SDL_GameController * gamepad)
{
	SDL_GameControllerClose(gamepad);
	Log::logI("Game Controller closed");
}

void CustomDestructors::DeleteSDL_Window(SDL_Window * window)
{
	SDL_DestroyWindow(window);
	Log::logI("Window Destroyed");
}

void CustomDestructors::DeleteSDL_Surface(SDL_Surface * surface)
{
	SDL_FreeSurface(surface);
	Log::logI("Surface Destroyed");
}
