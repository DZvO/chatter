#include "input.hpp"

Input::Input()
{
	//keystate = SDL_GetKeyState(NULL);
	close_requested = false;
	textmode = false;
	SDL_EnableUNICODE(1);
}

Input::~Input()
{
}

void Input::enableTextmode()
{
	//SDL_EnableUNICODE(1);
	textmode = true;
}

void Input::disableTextmode()
{
	//SDL_EnableUNICODE(0);
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

bool Input::isPressed (char16_t c)
{
	if(event.type == SDL_KEYDOWN && event.key.keysym.unicode == c)
		return true;
	return false;
}

bool Input::isPressedSym (char16_t k)
{
	return keystate[k];
}

bool Input::isPressed (Input::Key k)
{
	return SDL_GetKeyState(NULL)[k];
}

bool Input::isReleased (char16_t c)
{
	return !isPressed(c);
}

bool Input::isReleasedSym (char16_t k)
{
	return !isPressedSym(k);
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
			close_requested = true;
		else if(event.type == SDL_VIDEORESIZE)
			motor::Window::getInstance()->resize(event.resize.w, event.resize.h);
		else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		{
			bool pressed = (event.type == SDL_KEYDOWN ? true : false);
			auto unicode = event.key.keysym.unicode;
			auto sym = event.key.keysym.sym;
		}
	}
	return rv;
	//TODO handle mouse
}

void Input::addMapping (std::string name, char16_t k)
{
	keymap[name] = k;
}

char16_t Input::getMapping (std::string name)
{
	return keymap[name];
}
