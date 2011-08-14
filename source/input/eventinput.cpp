#include "eventinput.hpp"

EventInput::EventInput()
{
	keystate = SDL_GetKeyState(NULL);
}

EventInput::~EventInput()
{
}

const char* EventInput::getKeyState()
{
	return keystate;
}

bool EventInput::closeReq()
{
	refresh();
	return close_requested;
}

//private
void EventInput::refresh()
{
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
		{
			close_requested = true;
			SDL_Quit();
		}
	}
}
