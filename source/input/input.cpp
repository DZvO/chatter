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
		std::cout << "Whoow dude, getChar() was called while textmode was disabled! :s" << std::endl;
		return 0;
	}
}

const unsigned char* Input::getKeyState()
{
	return keystate;
}

bool Input::isPressedSym(Key k)
{
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

bool Input::closeRequested()
{
	return close_requested;
}

//private
int Input::refresh()
{
	//if(textmode)//return directly
	//{
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
	/*}
		else//loop through all the events, 'keystate' already gives information about pressed keys so we dont worry about them
		{
		while(SDL_PollEvent(&event))
		{
		if(event.type == SDL_QUIT)
		{
		close_requested = true;
		SDL_Quit();
		}
	//TODO handle mouse
	}
	return 0;
	}
	*/
}
