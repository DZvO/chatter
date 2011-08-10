#include <iostream>
using namespace std;

#include "graphics/window.hpp"

int main (int argc, char * argv[])
{
	Window * window = new Window();
	window->create(800, 600, "inspector gadget!");

	window->close();
	delete window;
	return 0;
}
