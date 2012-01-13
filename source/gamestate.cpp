#include "gamestate.hpp"
motor::state::GameState::GameState ()
{
	background = new Image("data/background.png");
	tiles = new Image("data/tiles.png");
	player.position = Vector2(0, 0);
	player.hitbox.width = 8;
	player.hitbox.height = 8;

	Item item;
	item.position = Vector2(100, 100);
	item.id = MANA_POTION;
	item.hitbox = Rectangle(0, 0, 6, 6);
	items.push_back(item);

	item.position = Vector2(200, 100);
	item.id = LIFE_POTION;
	items.push_back(item);
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

	player.update();
	if(input->isPressedSym("left"))
	{
		player.velocity.x -= 1.0 * (Window::getInstance()->getFrametime() / 10);
		player.direction.x -= 10.0;// * (Window::getInstance()->getFrametime());
	}
	if(input->isPressedSym("right"))
	{
		player.velocity.x += 1.0 * (Window::getInstance()->getFrametime() / 10);
		player.direction.x += 10.0;// * (Window::getInstance()->getFrametime());
	}
	if(input->isPressedSym("up"))
	{
		player.velocity.y -= 1.0 * (Window::getInstance()->getFrametime() / 10);
		player.direction.y -= 10.0;// * (Window::getInstance()->getFrametime());
	}
	if(input->isPressedSym("down"))
	{
		player.velocity.y += 1.0 * (Window::getInstance()->getFrametime() / 10);
		player.direction.y += 10.0;// * (Window::getInstance()->getFrametime());
	}

	if(input->isPressedSym(Input::kEnter) && player.laser_cooldown <= 0.0)
	{
		shootLazorBEAM(player);
	}

	for(auto & blt : bullets)
	{
		Bullet prev(blt);
		blt.position += blt.velocity * Vector2(Window::getInstance()->getFrametime() / 10.0f, Window::getInstance()->getFrametime() / 10.0f);
		Bullet now(blt);

		for(auto itr = items.begin(); itr != items.end(); )
		{
			auto & item = *itr;
			if(item.getHitbox().intersectsline(prev.getHitbox().getCenter(), now.getHitbox().getCenter()))
			{
				itr = items.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}

	if(player.laser_cooldown > 0.0)
	{
		player.laser_cooldown -= Window::getInstance()->getFrametime() / 1000.0;
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

	sb->draw(*tiles, Rectangle(0, Window::getInstance()->getHeight() - 7, 7 * 2, 7), Rectangle(0, 10, 7, 7), Vector4(1.0), 0.0, Vector2(0), 1, 3);

	for(Item & ent : items)
	{
		if(ent == LIFE_POTION)
		{
			sb->draw(*tiles, Rectangle(ent.position.x, ent.position.y, ent.hitbox.width, ent.hitbox.height), Rectangle(0, 9, 6, 6), Vector4(1.0), 0.0, Vector2(0, 0), 3.0, 2);
		}
		if(ent == MANA_POTION)
		{
			sb->draw(*tiles, Rectangle(ent.position.x, ent.position.y, ent.hitbox.width, ent.hitbox.height), Rectangle(7, 9, 6, 6), Vector4(1.0), 0.0, Vector2(0, 0), 3.0, 2);
		}
	}
	sb->draw(*tiles, Rectangle(player.position.x, player.position.y, 9, 8), Rectangle(0, 0, 9, 8), Vector4(1.0), 0, Vector2(4, 7), 4.0, 3);

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

void motor::state::GameState::shootLazorBEAM (Player & player)
{
	player.laser_cooldown = 0.3;
	Bullet blt;
	blt.hitbox.width = blt.hitbox.height = 4;
	blt.position = player.hitbox.getCenter() - Vector2(4/2, 4/2);// + Vector2(6,3) + (player.direction * Vector2(0.5,0.5));
	blt.velocity = glm::normalize(player.direction) * Vector2(1,1);
	bullets.push_back(blt);
}
