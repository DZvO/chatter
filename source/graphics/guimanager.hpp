#ifndef _GUIMANAGER_HPP
#define _GUIMANAGER_HPP

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "math/rectangle.hpp"
#include "graphics/spritebatch.hpp"
#include "graphics/image.hpp"

class GuiManager
{
	public:
		GuiManager ();
		void addButton (std::string text, Rectangle dest, Rectangle hitbox, void(*event)(void));
		void draw (SpriteBatch * sb, int mouseX, int mouseY);

		static void mouseCallback (void * instance, int x, int y, bool pressed);
	private:
		struct Button
		{
			std::string text;
			Rectangle rectangle;
			Rectangle hitbox;
			bool isPressed;
			void(*callback)(void);
		};
		std::vector <Button> buttons;
		Image * guiset;
		Image * font;
};

#endif
