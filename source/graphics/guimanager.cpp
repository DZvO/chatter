#include "guimanager.hpp"
//std::vector <GuiManager::Button> GuiManager::buttons;

GuiManager::GuiManager ()
{
	guiset = new Image("data/guiset.png");
	font = new Image("data/font.png");
}

void GuiManager::addButton (std::string text, Rectangle dest, Rectangle hitbox, void(*event)(void))
{
	Button btn;
	btn.text = text;
	btn.rectangle = dest;
	btn.hitbox = hitbox;
	btn.callback = event;
	//btn.isHighlighted = false;
	btn.isPressed = false;
	buttons.push_back(btn);
}

void GuiManager::draw (SpriteBatch * sb, int mouseX, int mouseY)
{
	for(auto & btn : buttons)
	{
		Rectangle uv;
		if(btn.isPressed)
		{
			//sb->draw(*guiset, Rectangle(btn.rectangle.x, btn.rectangle.y, btn.rectangle.width, btn.rectangle.height), Rectangle(0, 0, guiset->getWidth(), 170), Vector4(1, 1, 1, 1));
			uv.x = 0;
			uv.y = 0;
			uv.width = guiset->getWidth();
			uv.height = 170;
		}
		else if(btn.hitbox.isInside(mouseX, mouseY))
		{
			//sb->draw(*guiset, Rectangle(btn.rectangle.x, btn.rectangle.y, btn.rectangle.width, btn.rectangle.height), Rectangle(0, 170, guiset->getWidth(), 170), Vector4(1, 1, 1, 1));
			uv.x = 0;
			uv.y = 170;
			uv.width = guiset->getWidth();
			uv.height = 170;
		}
		else
		{
			//sb->draw(*guiset, Rectangle(btn.rectangle.x, btn.rectangle.y, btn.rectangle.width, btn.rectangle.height), Rectangle(0, 170*2, guiset->getWidth(), 170), Vector4(1, 1, 1, 1));
			uv.x = 0;
			uv.y = 170*2;
			uv.width = guiset->getWidth();
			uv.height = 170;
		}

		sb->draw(*guiset, Rectangle(btn.rectangle.x, btn.rectangle.y, btn.rectangle.width, btn.rectangle.height), uv, Vector4(1.0), 0.0, Vector2(0.0, 0.0), 1.0, 0);
		sb->drawString(*font, btn.text, Vector2(btn.rectangle.x, btn.rectangle.y), Vector4(1), true, 1.0, 1);
	}
}

void GuiManager::mouseCallback (void * instance, int x, int y, bool pressed)
{
	//std::cout.setf(std::ios::boolalpha);
	//std::cout << "mouse called back: " << x << " " << y << " " << pressed << '\n';

		//std::vector <Button> buttons;
	//for(auto itr = ((GuiManager*)instance)->buttons.begin(); itr != ((GuiManager*)instance)->buttons.end(); ++itr)
	for(auto & btn : ((GuiManager*)instance)->buttons)
	{
		if(pressed == false)
			btn.isPressed = false;
		else if(btn.hitbox.isInside(x, y))
		{
			btn.isPressed = true;
			(*(btn.callback))();
		}
	}
}
