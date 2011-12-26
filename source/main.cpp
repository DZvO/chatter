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
#include "time/stopwatch.hpp"
#include "input/input.hpp"
#include "helper.hpp"

#include "graphics/camera.hpp"
#include "graphics/spritebatch.hpp"
#include "graphics/guimanager.hpp"

int main (int argc, char * argv[])
{
	Window::getInstance()->create(1366, 768, "inspector gadget!", true);
	StopWatch * stopwatch = new StopWatch();
	SpriteBatch * sb = new SpriteBatch();
	unsigned int time = SDL_GetTicks();

	bool enable_textinput = false;
	Input * input = new Input();
	if(ifstream("data/preferences.ini"))
		input->loadKeymapping("data/preferences.ini");
	else
	{
		input->addMapping("escape", SDLK_ESCAPE);
		input->saveKeymapping("data/preferences.ini");
	}

	/*while(input->closeRequested() == false)
	{
		while(input->refresh() != 0) // pump events like crazy!
		{
			if(input->isPressed(input->getMapping("escape")))
				return 0; //os will free memory anyways, so omit it here
		}

		Window::getInstance()->clear();
		stopwatch->start();

		state->run();

		stopwatch->stop();
		Window::getInstance()->swap();

		if(time < SDL_GetTicks())
		{
			time = SDL_GetTicks() + 500;

			std::cout.setf(ios::fixed, ios::floatfield);
			std::cout.setf(ios::showpoint);
			std::cout.width(6);
			std::cout.precision(3);
			std::cout << stopwatch->get() << " ms"<< '\n';
		}
	}
	Window::getInstance()->close();*/

	

	delete input;
	delete sb;
	delete stopwatch;
	return 0;
}

// vim:foldmethod=marker
