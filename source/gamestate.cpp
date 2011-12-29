#include "gamestate.hpp"
motor::state::GameState::GameState () : player_acceleration(Vector2(1.0))
{
	background = new Image("data/background.png");
	tiles = new Image("data/tiles.png");
	player_position = Vector2(0.0);
	player_velocity = Vector2(0.0);
}

void motor::state::GameState::init ()
{
}

void motor::state::GameState::update ()
{
	Input * input = Input::getInstance();
	if(input->closeRequested())
		StateManager::getInstance()->stop();
	while(input->refresh() != 0)
	{
		if(input->isPressed("escape"))
			StateManager::getInstance()->stop();
	}

	if(input->isPressedSym("left"))
		player_velocity.x -= player_acceleration.x * (Window::getInstance()->getFrametime() / 10);
	if(input->isPressedSym("right"))
		player_velocity.x += player_acceleration.x * (Window::getInstance()->getFrametime() / 10);
	if(input->isPressedSym("up"))
		player_velocity.y -= player_acceleration.y * (Window::getInstance()->getFrametime() / 10);
	if(input->isPressedSym("down"))
		player_velocity.y += player_acceleration.y * (Window::getInstance()->getFrametime() / 10);
	if(input->isPressedSym(Input::kEnter))
		shootLazorBEAM(Vector2(0,0));

	player_position += player_velocity;
	if(glm::length(player_velocity) < 0.001)
		player_velocity = Vector2(0,0);
	else
		player_velocity *= 0.8;

	if(player_velocity != Vector2(0,0))
	{
		Vector2 nv = glm::normalize(player_velocity);
		Vector2 dir = glm::normalize(Vector2(0, -1));
		player_rotation = -(glm::atan(nv.y, nv.x) - glm::atan(dir.y, dir.x));
		if(player_rotation != player_rotation) //check for NaN
			player_rotation = 0;
	}
	cout << player_rotation << '\n';

	for(auto & blt : bullets)
	{
		blt.position += blt.velocity;
		blt.velocity += glm::normalize(blt.velocity);
	}
}

void motor::state::GameState::draw ()
{
	Window::getInstance()->clear();
	SpriteBatch * sb = StateManager::getInstance()->getSpriteBatch();
	sb->begin();

	sb->draw(*background,
			Rectangle(-Window::getInstance()->getWidth(), -Window::getInstance()->getHeight(), Window::getInstance()->getWidth() * 2, Window::getInstance()->getHeight() * 2),
			Rectangle(player_position.x, player_position.y, Window::getInstance()->getWidth() * 3, Window::getInstance()->getHeight() * 3),
			Vector4(1.0),
			0.0,
			Vector2(0.0),
			1.0,
			1);

	sb->draw(*tiles, Rectangle(player_position.x, player_position.y, 12, 12), Rectangle(0, 0, 12, 12), Vector4(1.0), (player_rotation), Vector2(6, 6), 1.0, 2);

	for(auto itr = bullets.begin(); itr != bullets.end(); itr++)
	{
		if(Rectangle(0, 0, Window::getInstance()->getWidth(), Window::getInstance()->getHeight()).isInside(itr->position.x, itr->position.y) == false)
		{
			bullets.erase(itr);
			itr--;
		}
		else
		{
			sb->draw(*tiles,
					Rectangle(itr->position.x, itr->position.y, 3, 13),
					Rectangle(13, 0, 3, 13),
					Vector4(1.0),
					itr->rotation,
					Vector2(3/2, 13/2),
					1.0,
					3);
		}
	}

	sb->end();
	Window::getInstance()->swap();
}

void motor::state::GameState::shootLazorBEAM (Vector2 nDirection)
{
	bullet blt;
	blt.position = player_position + Vector2(6,0) + (glm::normalize(player_velocity) * Vector2(20,20));
	blt.velocity = player_velocity;
	Vector2 nv = glm::normalize(blt.velocity);
	Vector2 dir = glm::normalize(Vector2(0, -1));
	blt.rotation = -(glm::atan(nv.y, nv.x) - glm::atan(dir.y, dir.x));
	bullets.push_back(blt);
}
