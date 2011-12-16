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
#include "graphics/spriteBatch.hpp"
#include "graphics/guimanager.hpp"

enum STATE { ADDRESS_ENTRY, ACK_WAIT, NAME_ENTRY, NORMAL} state = ADDRESS_ENTRY;

void buttonPressed ()
{
	exit(0);
}

int main (int argc, char * argv[])
{
	Window::getInstance()->create(1366, 768, "inspector gadget!", true);
	StopWatch * stopwatch = new StopWatch();
	SpriteBatch * sb = new SpriteBatch();
	Image cubetex ("data/cubetex.png");
	GuiManager guimanager;// = new GuiManager();
	unsigned int time = SDL_GetTicks();

	bool enable_textinput = false;
	Input * input = new Input();
	if(ifstream("data/preferences.ini"))
	{
		input->loadKeymapping("data/preferences.ini");
	}
	else
	{
		input->addMapping("escape", SDLK_ESCAPE);
		input->saveKeymapping("data/preferences.ini");
	}

	input->setMouseCallback(&guimanager, GuiManager::mouseCallback);
	guimanager.addButton("Button Text", Rectangle(100, 100, 50, 50), buttonPressed);

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
		stopwatch->start();

		sb->begin();
		//for(int i = 0; i < 100; i++)
			//sb->draw(cubetex, Rectangle(100, 100, 50, 50), Rectangle(0, 0, cubetex.getWidth(), cubetex.getHeight()), Vector4(1, 1, 1, 1));
		guimanager.draw(sb, input->getMouseX(), input->getMouseY());
		sb->end();

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
	Window::getInstance()->close();
	delete input;
	delete sb;
	delete stopwatch;
	return 0;
}

// vim:foldmethod=marker
