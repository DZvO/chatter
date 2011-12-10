#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "lib/glew/glew.h"
#include <SDL/SDL.h>

#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtx/projection.hpp"
#include "lib/glm/gtc/type_ptr.hpp"

#include "graphics/window.hpp"
using namespace motor;
#include "input/input.hpp"
#include "helper.hpp"

#include "graphics/camera.hpp"
#include <sys/time.h>

#include "graphics/spriteBatch.hpp"

enum STATE { ADDRESS_ENTRY, ACK_WAIT, NAME_ENTRY, NORMAL} state = ADDRESS_ENTRY;

int main (int argc, char * argv[])
{
	Window::getInstance()->create(1366, 768, "inspector gadget!", true);
	SpriteBatch * sb = new SpriteBatch();
	Image cubetex ("data/cubetex.png");
	cubetex.upload();
	unsigned int time = SDL_GetTicks();

	bool enable_textinput = false;
	Input * input = new Input();
	if(ifstream("data/preferences.ini"))
	{
		input->loadKeymapping("data/preferences.ini");
	}
	else
	{
		input->addMapping("forward", (SDLKey)44);
		input->addMapping("backward", (SDLKey)111);
		input->addMapping("left", (SDLKey)97);
		input->addMapping("right", (SDLKey)101);
		input->addMapping("escape", SDLK_ESCAPE);
		input->saveKeymapping("data/preferences.ini");
	}

	timeval tv;
	unsigned long long delta = 0;
	unsigned long long avg = 0;
	int delta_count = 0;

	while(input->closeRequested() == false)
	{
		while(input->refresh() != 0) // pump events like crazy!
		{
			if(input->isPressed(input->getMapping("escape")))
			{
				return 0; //os will free memory anyways, so omit it here
			}
		}

		Window::getInstance()->clear();
		gettimeofday(&tv, nullptr);
		delta = tv.tv_usec;

		sb->begin();
		for(int i = 0; i < 10000; i++)
			sb->draw(cubetex, Rectangle(100 + i, 100 + i, cubetex.getWidth(), cubetex.getHeight()), Rectangle(0, 0, cubetex.getWidth(), cubetex.getHeight()), Vector4(1, 1, 1, 1));
		sb->end();

		gettimeofday(&tv, nullptr);
		delta = tv.tv_usec - delta;
		//delta_count++;
		//avg += delta;
		Window::getInstance()->swap();

		if(time < SDL_GetTicks())
		{
			time = SDL_GetTicks() + 500;

			std::cout.setf(ios::fixed, ios::floatfield);
			std::cout.setf(ios::showpoint);
			//std::cout << (float(avg) / float(delta_count)) / 1000.0f << '\n';
			std::cout << float(delta) / 1000.0f << '\n';
		}
	}

	Window::getInstance()->close();
	delete input;
	return 0;
}

// vim:foldmethod=marker
