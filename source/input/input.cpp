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

unsigned char Input::getChar()
{
	if(textmode)
	{
		if(refresh())
		{
			//maybe check for keydown? because now both keydown and keyup events are handled
			return event.key.keysym.unicode;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		std::cout << "Whoow dude, getChar() was called while textmode was disabled! :s" << endl;
		return 0;
	}
}

const unsigned char* Input::getKeyState()
{
	return keystate;
}

bool Input::isPressed(Key k)
{
	return (keystate[k]);
}

bool Input::isReleased(Key k)
{
	return !isPressed(k);
}

bool Input::closeReq()
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
			SDL_Quit();
		}
	}
	return rv;
}
