#include "input.hpp"

Input::Input()
{
	keystate = SDL_GetKeyState(NULL);
	close_requested = false;
	textmode = false;
}

Input::~Input()
{
}

void Input::enableTextmode()
{
	SDL_EnableUNICODE(1);
	textmode = true;
}

void Input::disableTextmode()
{
	SDL_EnableUNICODE(0);
	textmode = false;
}

void Input::enableKeyRepeat ()
{
	SDL_EnableKeyRepeat(1500, 50);
}

void Input::disableKeyRepeat ()
{
	SDL_EnableKeyRepeat(0, 0);
}

unsigned char Input::getChar()
{
	if(event.type == SDL_KEYDOWN)
		return event.key.keysym.unicode;
	else // dont handle ""
		return 0;
}

const unsigned char* Input::getKeyState()
{
	keystate = SDL_GetKeyState(NULL);
	return keystate;
}

bool Input::isPressedSym(Key k)
{
	keystate = SDL_GetKeyState(NULL);
	return (keystate[k] == 1 ? true : false);
}

bool Input::isPressed(Key k)
{
	if(event.type == SDL_KEYDOWN && event.key.keysym.sym == k)
		return true;
	return false;
}

bool Input::isReleasedSym(Key k)
{
	return !isPressedSym(k);
}

bool Input::isReleased(Key k)
{
	return !isPressed(k);
}

void Input::requestClose()
{
	close_requested = true;
}

bool Input::closeRequested()
{
	return close_requested;
}

//private
int Input::refresh()
{
	int rv = SDL_PollEvent(&event);
	if(rv == 1)//if there are pending events
	{
		if(event.type == SDL_QUIT)
		{
			close_requested = true;
		}
	}
	return rv;
	//TODO handle mouse
}
