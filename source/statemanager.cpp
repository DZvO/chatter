#include "statemanager.hpp"

StateManager * motor::StateManager::getInstance ()
{
	static StateManager sm;
	return &sm;
}

void motor::StateManager::changeState (motor::state::State * newState)
{
	delete currentState;
	currentState = newState;
	currentState->init();
}

bool motor::StateManager::isRunning ()
{
	return running;
}

void motor::StateManager::stop ()
{
	running = false;
}

void motor::StateManager::run ()
{
	if(currentState != nullptr)
	{
		currentState->update();
		currentState->draw();
	}
}

SpriteBatch * motor::StateManager::getSpriteBatch ()
{
	return &spritebatch;
}

motor::StateManager::StateManager ()
{
	currentState = nullptr;
	running = true;
}
