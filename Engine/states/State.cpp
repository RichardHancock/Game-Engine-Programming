#include "State.h"

State::State(StateManager* stateManager, Platform* platform, 
	ResourceManager* resourceManager, InputManager* inputManager)
		: stateManager(stateManager), platform(platform), 
		resourceManager(resourceManager), inputManager(inputManager)
{
	
}