#include "gamestate.hpp"
motor::state::GameState::GameState ()
{
	background = new Image("data/background.png");
	tiles = new Image("data/tiles.png");
	spaceship.hitbox.width = 12;
	spaceship.hitbox.height = 12;

	Entity beacon;
	beacon.position = Vector2(100, 100);
	beacon.id = MANA_POTION;
	beacon.hitbox = Rectangle(0, 0, 9, 13);
	entitys.push_back(beacon);

	beacon.position = Vector2(200, 100);
	entitys.push_back(beacon);
	std::cout << "created GameState" << '\n';
}

motor::state::GameState::~GameState ()
{
	std::cout << "deleted GameState" << '\n';
}

void motor::state::GameState::init (const motor::StateManager * st)
{
}

void motor::state::GameState::update (const motor::StateManager * st)
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
		spaceship.velocity.x -= 1.0 * (Window::getInstance()->getFrametime() / 10);
	if(input->isPressedSym("right"))
		spaceship.velocity.x += 1.0 * (Window::getInstance()->getFrametime() / 10);
	if(input->isPressedSym("up"))
		spaceship.velocity.y -= 1.0 * (Window::getInstance()->getFrametime() / 10);
	if(input->isPressedSym("down"))
		spaceship.velocity.y += 1.0 * (Window::getInstance()->getFrametime() / 10);
	if(input->isPressedSym(Input::kEnter) && spaceship.laser_cooldown <= 0.0)
	{
		shootLazorBEAM(spaceship);
	}

	for(auto & blt : bullets)
	{
		blt.position += blt.velocity;
		blt.hitbox.x += blt.velocity.x;
		blt.hitbox.y += blt.velocity.y;
		blt.velocity += glm::normalize(blt.velocity);
	}
	if(spaceship.laser_cooldown > 0.0)
	{
		spaceship.laser_cooldown -= Window::getInstance()->getFrametime() / 1000.0;
	}
	spaceship.update();

	for(auto itr = entitys.begin(); itr != entitys.end(); itr++)
	{
		auto & ent = *itr;

		for(auto & blt : bullets)
		{
			if(blt.hitbox.collides(Rectangle(ent.position.x, ent.position.y, ent.hitbox.width, ent.hitbox.height)))
			{
				cout << "AAHHH IM HIT!\n";
			}
		}

		if(spaceship.hitbox.collides(Rectangle(ent.position.x, ent.position.y, ent.hitbox.width, ent.hitbox.height)))
		{
			if(ent == DEBUG_POTION)
				cout << " debug potion \n";
			else if (ent == MANA_POTION)
			{
				spaceship.position += Vector2(100, 100);
			}
			itr = entitys.erase(itr);
		}
	}
}

void motor::state::GameState::draw (const motor::StateManager * st)
{
	Window::getInstance()->clear();
	//SpriteBatch * sb = StateManager::getInstance()->getSpriteBatch();
	SpriteBatch * sb = const_cast<motor::StateManager *> (st)->getSpriteBatch();
	sb->begin();

	sb->draw(*background,
			Rectangle(-Window::getInstance()->getWidth(), -Window::getInstance()->getHeight(), Window::getInstance()->getWidth() * 2, Window::getInstance()->getHeight() * 2),
			Rectangle(spaceship.position.x, spaceship.position.y, Window::getInstance()->getWidth() * 3, Window::getInstance()->getHeight() * 3),
			Vector4(1.0),
			0.0,
			Vector2(0.0),
			1.0,
			1);

	sb->draw(*tiles, Rectangle(spaceship.position.x, spaceship.position.y, 12, 12), Rectangle(0, 0, 12, 12), Vector4(1.0), (spaceship.rotation), Vector2(6, 6), 1.0, 2);
	for(Entity & ent : entitys)
		sb->draw(*tiles, Rectangle(ent.position.x, ent.position.y, ent.hitbox.width, ent.hitbox.height), Rectangle(18, 0, 9, 13), Vector4(1.0), 0.0, Vector2(0, 0), 1.0, 2);

	for(auto itr = bullets.begin(); itr != bullets.end(); itr++)
	{
		if(Rectangle(0, 0, Window::getInstance()->getWidth(), Window::getInstance()->getHeight()).isInside(itr->position.x, itr->position.y) == false)
		{
			bullets.erase(itr);
			itr--;
		}
		else
		{
			sb->draw(*tiles, Rectangle(itr->hitbox.x, itr->hitbox.y, 3, 3), Rectangle(0, 12, 3, 3), Vector4(1.0), 0.0, Vector2(0), 1.0, 4);
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

void motor::state::GameState::shootLazorBEAM (SpaceShip & spaceship)
{
	spaceship.laser_cooldown = 0.3;
	bullet blt;
	blt.position = spaceship.position + Vector2(6,0) + (spaceship.direction * Vector2(20,20));
	blt.velocity = spaceship.direction * Vector2(10, 10);
	Vector2 nv = spaceship.direction;
	Vector2 dir = glm::normalize(Vector2(0, -1));
	blt.rotation = -(glm::atan(nv.y, nv.x) - glm::atan(dir.y, dir.x));
	blt.hitbox.x = blt.position.x + blt.velocity.x;
	blt.hitbox.y = blt.position.y + blt.velocity.y;
	blt.hitbox.width = 3;
	blt.hitbox.height = 3;
	bullets.push_back(blt);
}
