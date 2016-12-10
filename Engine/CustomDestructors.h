#pragma once

#include <SDL.h>

namespace CustomDestructors
{
	struct SDL_Deleter
	{
		void operator()(SDL_GameController* gamepad);
		void operator()(SDL_Haptic* haptic);
		
	};
	
}