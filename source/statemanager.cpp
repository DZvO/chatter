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
	currentState->init(this);
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
	while(running)
	{
		if(currentState != nullptr)
		{
			currentState->update(this);
			currentState->draw(this);
		}
		ticks++;
		if(ticks % 100 == 0)
		{
			cout << "[" << Window::getInstance()->getElapsedTime() << "] " << "gametime : \t" << Window::getInstance()->getElapsedTime() << " ms\n";
			cout << "[" << Window::getInstance()->getElapsedTime() << "] " << "frametime: \t" << Window::getInstance()->getFrametime() << " ms\n";
		}
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
