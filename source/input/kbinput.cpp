#include "eventinput.hpp"

KbInput::KbInput()
{
	keystate = SDL_GetKeyState(NULL);
}

KbInput::~KbInput()
{
}

void KbInput::enableTextmode()
{
	SDL_EnableUNICODE(1);
}

void KbInput::disableTextmode()
{
	SDL_EnableUNICODE(0);
}

char KbInput::getChar()
{
	return event.key.keysym.unicode;
}

bool KbInput::inputAvailable()
{
	if(refresh())
		return true;
	else
		return false;
}

Key KbInput::getKey()
{
	//if(event.key.state == SDL_PRESSED)
		return event.key.keysym.sym;
	//else
		//return Key::null;
}

const char* KbInput::getKeyState()
{
	return keystate;
}

bool KbInput::isPressed(Key k)
{
	return (keystate[k]);
}

bool KbInput::isReleased(Key k)
{
	return !isPressed(k);
}

bool KbInput::closeReq()
{
	return close_requested;
}

//private
int KbInput::refresh()
{
	int rv = SDL_PollEvent(&event);
	if(rv == 1)//if there are pending events
	{
		if(event.type == SDL_QUIT)
		{
			close_requested = true;
			SDL_Quit();
		}
	}
	return rv;
}
