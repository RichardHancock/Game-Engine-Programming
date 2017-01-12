#include "StateManager.h"
#include "State.h"
#include "../misc/Utility.h"
#include "../misc/Log.h"

std::vector<std::shared_ptr<State>> StateManager::states;

void StateManager::cleanup()
{
	clearStates();
}

void StateManager::clearStates()
{
	states.clear();
}

void StateManager::addState(std::shared_ptr<State> state)
{
	states.push_back(state);
	Log::logI("StateManager: Switched to the " + state->getStateName());
}

void StateManager::prepareToChangeState()
{
	Utility::Timer::cleanup();
}

void StateManager::changeState(std::shared_ptr<State> state)
{
	clearStates();
	addState(state);
}

void StateManager::popLastState()
{
	states.pop_back();
}

bool StateManager::eventHandler()
{
	return states.back()->eventHandler();
}

void StateManager::update()
{
	states.back()->update();
}

void StateManager::render()
{
	for (int i = 0; i < (int)states.size(); i++)
	{
		states[i]->render();
	}
}