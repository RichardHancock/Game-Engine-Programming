#include "CustomDestructors.h"

#include "misc/Log.h"

void CustomDestructors::SDL_Deleter::operator()(SDL_GameController* gamepad)
{
	SDL_GameControllerClose(gamepad);
}

void CustomDestructors::SDL_Deleter::operator()(SDL_Haptic* haptic)
{
	SDL_HapticClose(haptic);
}
