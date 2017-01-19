#include "CustomDestructors.h"

#include "Log.h"

void CustomDestructors::SDL_Deleter::operator()(SDL_GameController* gamepad)
{
	SDL_GameControllerClose(gamepad);
	gamepad = nullptr;
}

void CustomDestructors::SDL_Deleter::operator()(SDL_Haptic* haptic)
{
	SDL_HapticClose(haptic);
	haptic = nullptr;
}

void CustomDestructors::SDL_Deleter::operator()(SDL_Window * window)
{
	SDL_DestroyWindow(window);
	window = nullptr;
}

void CustomDestructors::SDL_Deleter::operator()(SDL_GLContext context)
{
	SDL_GL_DeleteContext(context);
}

void CustomDestructors::SDL_Deleter::operator()(SDL_Context* context)
{
	if (context->glContext != nullptr)
	{
		SDL_GL_DeleteContext(context->glContext);
		context->glContext = nullptr;
	}

	if (context->window != nullptr)
	{
		SDL_DestroyWindow(context->window);
		context->window = nullptr;
	}

	if (context != nullptr)
	{
		delete context;
	}
}
