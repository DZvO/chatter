#include "menustate.hpp"

motor::state::MenuState::MenuState ()
{
	std::cout << "created MenuState" << '\n';
	image = new Image("data/menustate.png");
	font = new Image("data/font.png");
}

motor::state::MenuState::~MenuState ()
{
	std::cout << "destroyed MenuState" << '\n';
}

void motor::state::MenuState::init (const motor::StateManager * state)
{
}

void motor::state::MenuState::update (const motor::StateManager * state)
{
	Input * input = Input::getInstance();
	if(input->closeRequested())
		StateManager::getInstance()->stop();
	while(input->refresh() != 0)
	{
		if(input->isPressed("enter"))
		{
			StateManager::getInstance()->changeState(new GameState());
			return;
		}
		else if(input->isPressed("escape"))
		{
			StateManager::getInstance()->changeState(new KeyboardSettingsState());
			return;
		}
	}
	ticks++;
}

void motor::state::MenuState::draw (const motor::StateManager * state)
{
	SpriteBatch * sb = const_cast<motor::StateManager*>(state)->getSpriteBatch();
	Window::getInstance()->clear();
	sb->begin();
	sb->drawString(*font, "SPACE!",  Vector2(100, 100), true, 4.0, 1);
	sb->drawString(*font, "Press Enter to continue!",  Vector2(100, 140), true, 2.0, 1);
	sb->drawString(*font, "or press escape to configure the keyboardlayout!",  Vector2(100, 160), true, 2.0, 1);
	sb->end();
	Window::getInstance()->swap();
}
