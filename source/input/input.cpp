#include "input.hpp"

Input::Input()
{
	//keystate = SDL_GetKeyState(NULL);
	close_requested = false;
	textmode = false;
	SDL_EnableUNICODE(1);
	//mousecallback = nullptr;
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

bool Input::isPressed (Input::Key k)
{
	if(event.type == SDL_KEYDOWN && event.key.keysym.sym == (SDLKey)k)
		return true;
	return false;
}

bool Input::isPressedSym (Input::Key k)
{
	return SDL_GetKeyState(NULL)[k];
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
			//keystate[unicode] = pressed;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
		{
			int x = 0, y = 0;
			SDL_GetMouseState(&x, &y);
			//if(mousecallback != nullptr)
				//(*mousecallback)(x, y, (event.type == SDL_MOUSEBUTTONDOWN ? true : false));
			mousecallback(inst, x, y, (event.type == SDL_MOUSEBUTTONDOWN ? true : false));
		}
	}
	return rv;
	//TODO handle mouse
}

void Input::addMapping (std::string name, Input::Key k)
{
	keymap[name] = k;
}

void Input::addMapping (std::string name, SDLKey k)
{
	keymap[name] = (Input::Key)k;
}

Input::Key Input::getMapping (std::string name)
{
	return (Input::Key)keymap[name];
}

void Input::loadKeymapping (std::string path)
{
	std::fstream fs (path, std::fstream::in);
	while(fs.good())
	{
		std::string currentLine;
		std::getline(fs, currentLine);
		if(currentLine == "")
			break;
		std::string key, value;
		key = currentLine.substr(0, currentLine.find(" = "));
		value = currentLine.substr(currentLine.find(" = ") + string(" = ").length(), currentLine.length() - (currentLine.find(" = ") + string(" = ").length()));

		//convert 'value' to an int
		std::stringstream ss;
		ss << value;
		int val;
		ss >> val;

		keymap[key] = val;
	}
}

void Input::saveKeymapping (std::string path)
{
	std::fstream fs (path, std::fstream::trunc | std::fstream::out);
	for(auto k : keymap)
	{
		fs << k.first << " = " << k.second << '\n';
	}
}

void Input::setMouseCallback(void * instance, void(cb)(void *, int,int,bool))
{
	inst = instance;
	mousecallback = cb;
}

unsigned short Input::getMouseX ()
{
	int r = 0;
	SDL_GetMouseState(&r, nullptr);
	return r;
}

unsigned short Input::getMouseY ()
{
	int r = 0;
	SDL_GetMouseState(nullptr, &r);
	return r;
}
