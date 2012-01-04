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
	beacon.id = LIFE_POTION;
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

	spaceship.update();
	if(input->isPressedSym("left"))
	{
		spaceship.velocity.x -= 1.0 * (Window::getInstance()->getFrametime() / 10);
		spaceship.direction.x -= 10.0;// * (Window::getInstance()->getFrametime());
	}
	if(input->isPressedSym("right"))
	{
		spaceship.velocity.x += 1.0 * (Window::getInstance()->getFrametime() / 10);
		spaceship.direction.x += 10.0;// * (Window::getInstance()->getFrametime());
	}
	if(input->isPressedSym("up"))
	{
		spaceship.velocity.y -= 1.0 * (Window::getInstance()->getFrametime() / 10);
		spaceship.direction.y -= 10.0;// * (Window::getInstance()->getFrametime());
	}
	if(input->isPressedSym("down"))
	{
		spaceship.velocity.y += 1.0 * (Window::getInstance()->getFrametime() / 10);
		spaceship.direction.y += 10.0;// * (Window::getInstance()->getFrametime());
	}

	if(input->isPressedSym(Input::kEnter) && spaceship.laser_cooldown <= 0.0)
	{
		shootLazorBEAM(spaceship);
	}

	for(auto & blt : bullets)
	{
		bullet prev(blt);
		blt.position += blt.velocity * Vector2(Window::getInstance()->getFrametime() / 10.0f, Window::getInstance()->getFrametime() / 10.0f);
		bullet now(blt);

		for(auto itr = entitys.begin(); itr != entitys.end(); )
		{
			auto & ent = *itr;
			if(ent.getHitbox().intersectsline(prev.getHitbox().getCenter(), now.getHitbox().getCenter()))
			{
				itr = entitys.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}

	if(spaceship.laser_cooldown > 0.0)
	{
		spaceship.laser_cooldown -= Window::getInstance()->getFrametime() / 1000.0;
	}

	/*for(auto itr = entitys.begin(); itr != entitys.end(); itr++)
	{
		auto & ent = *itr;
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
	}*/
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
		if(Rectangle(-100, -100, Window::getInstance()->getWidth()+100*2, Window::getInstance()->getHeight()+100*2).isInside(itr->position.x, itr->position.y) == false)
		{
			bullets.erase(itr);
			itr--;
		}
		else
		{
			sb->draw(*tiles, Rectangle(itr->position.x, itr->position.y, itr->hitbox.width, itr->hitbox.height), Rectangle(0, 12, 4, 4), Vector4(1.0), 0.0, Vector2(0), 1.0, 4);
			/*sb->draw(*tiles,
					Rectangle(itr->position.x, itr->position.y, 3, 13),
					Rectangle(13, 0, 3, 13),
					Vector4(1.0),
					itr->rotation,
					Vector2(3/2, 13/2),
					1.0,
					3);*/
		}
	}

	sb->end();
	Window::getInstance()->swap();
}

void motor::state::GameState::shootLazorBEAM (SpaceShip & spaceship)
{
	spaceship.laser_cooldown = 0.3;
	bullet blt;
	blt.hitbox.width = blt.hitbox.height = 4;
	blt.position = spaceship.hitbox.getCenter() - Vector2(4/2, 4/2);// + Vector2(6,3) + (spaceship.direction * Vector2(0.5,0.5));
	blt.velocity = glm::normalize(spaceship.direction) * Vector2(1,1);
	bullets.push_back(blt);
}
