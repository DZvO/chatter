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

#include "statemanager.hpp"
#include "state.hpp"
#include "gamestate.hpp"
#include "menustate.hpp"

int main (int argc, char * argv[])
{
	Window::getInstance()->create(1366, 768, "inspector gadget!", true);

	Input * input = Input::getInstance();
	if(ifstream("data/preferences.ini"))
		input->loadKeymapping("data/preferences.ini");
	else
	{
		input->addMapping("escape", SDLK_ESCAPE);
		input->addMapping("up", SDLK_UP);
		input->addMapping("down", SDLK_DOWN);
		input->addMapping("left", SDLK_LEFT);
		input->addMapping("right", SDLK_RIGHT);
		input->saveKeymapping("data/preferences.ini");
	}

	StateManager * stateman = StateManager::getInstance();
	stateman->changeState(new state::MenuState());
	stateman->run();

	std::cout << "bye\n";
	return 0;
}

// vim:foldmethod=marker
