#ifndef _GUIMANAGER_HPP
#define _GUIMANAGER_HPP

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "math/rectangle.hpp"
#include "graphics/spriteBatch.hpp"
#include "graphics/image.hpp"

class GuiManager
{
	public:
		GuiManager ();
		void addButton (std::string text, Rectangle dest, void(*event)(void));
		void draw (SpriteBatch * sb, int mouseX, int mouseY);

		static void mouseCallback (void * instance, int x, int y, bool pressed);
	private:
		struct Button
		{
			std::string text;
			Rectangle rectangle;
			bool isPressed;
			void(*callback)(void);
		};
		std::vector <Button> buttons;
		Image * cubetex;
};

#endif
