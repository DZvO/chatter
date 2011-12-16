#include "guimanager.hpp"
//std::vector <GuiManager::Button> GuiManager::buttons;

GuiManager::GuiManager ()
{
	cubetex = new Image("data/cubetex.png");
}

void GuiManager::addButton (std::string text, Rectangle dest, void(*event)(void))
{
	Button btn;
	btn.text = text;
	btn.rectangle = dest;
	btn.callback = event;
	//btn.isHighlighted = false;
	btn.isPressed = false;
	buttons.push_back(btn);
}

void GuiManager::draw (SpriteBatch * sb, int mouseX, int mouseY)
{
	for(auto btn : buttons)
	{
		if(btn.isPressed)
		{
			sb->draw(*cubetex, Rectangle(100, 100, 50, 50), Rectangle(0, 0, cubetex->getWidth(), cubetex->getHeight()), Vector4(1, 0, 0, 1));
			//std::cout << " 1 \n";
		}
		else if(btn.rectangle.isInside(mouseX, mouseY))
		{
			sb->draw(*cubetex, Rectangle(100, 100, 50, 50), Rectangle(0, 0, cubetex->getWidth(), cubetex->getHeight()), Vector4(1, 1, 0, 1));
			//std::cout << " 2 \n";
		}
		else
		{
			sb->draw(*cubetex, Rectangle(100, 100, 50, 50), Rectangle(0, 0, cubetex->getWidth(), cubetex->getHeight()), Vector4(1, 1, 1, 1));
			//std::cout << " 3 \n";
		}
	}
}

void GuiManager::mouseCallback (void * instance, int x, int y, bool pressed)
{
	//std::cout.setf(std::ios::boolalpha);
	//std::cout << "mouse called back: " << x << " " << y << " " << pressed << '\n';

		//std::vector <Button> buttons;
	for(auto itr = ((GuiManager*)instance)->buttons.begin(); itr != ((GuiManager*)instance)->buttons.end(); ++itr)
	{
		if(pressed == false)
			itr->isPressed = false;
		else if(itr->rectangle.isInside(x, y))
		{
			itr->isPressed = true;
			(*(itr->callback))();
		}
	}
}
