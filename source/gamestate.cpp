#include "gamestate.hpp"
motor::state::GameState::GameState (): level(Vector2(800, 600))
{
	background = new Image("data/background.png");
	tiles = new Image("data/tiles.png");
	player.position = Vector2(0, 200);
	player.hitbox.width = 9;
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

	Rectangle prev = player.getHitbox();
	player.update();
	float xSpeed = 0.1, ySpeed = 8.0;
	if(input->isPressedSym("left"))
	{
		player.velocity.x -= xSpeed * (Window::getInstance()->getFrametime() / 10);
		player.direction.x -= 10.0;// * (Window::getInstance()->getFrametime());
	}
	if(input->isPressedSym("right"))
	{
		player.velocity.x += xSpeed * (Window::getInstance()->getFrametime() / 10);
		player.direction.x += 10.0;// * (Window::getInstance()->getFrametime());
	}
	if(input->isPressedSym("up") && player.flying == false && player.jump_cooldown <= 0.0)
	{
		player.velocity.y -= ySpeed * 1.2;//* (Window::getInstance()->getFrametime() / 10);
		player.direction.y -= 10.0;// * (Window::getInstance()->getFrametime());
		player.jump_cooldown = 0.5f;
		cout << "jump" << "\n";
	}
	if(input->isPressedSym("down"))
	{
		player.velocity.y += ySpeed * (Window::getInstance()->getFrametime() / 10);
		player.direction.y += 10.0;// * (Window::getInstance()->getFrametime());
	}
	

	//collision detection
	{
		Rectangle staticGeom = Rectangle(0, Window::getInstance()->getHeight() - 8*2, 8*4*2, 8*2);
		//clipping
		if(staticGeom.intersectsline(prev.getLowerLeft(), player.getHitbox().getLowerLeft()) ||
				staticGeom.intersectsline(prev.getLowerRight(), player.getHitbox().getLowerRight()))
		{
			if(player.velocity.y > 0.0)
			{
				player.velocity.y = 0;
				player.position.y = staticGeom.y;
				player.flying = false;
				cout << "collides" << '\n';
			}
		}
		//flying
		else
		{
			if(player.velocity.y < 6.0)
				player.velocity.y += 0.80;
				//player.velocity.y += 0.4;
			player.flying = true;
		}
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
}

void motor::state::GameState::draw (const motor::StateManager * st)
{
	Window::getInstance()->clear();
	//SpriteBatch * sb = StateManager::getInstance()->getSpriteBatch();
	SpriteBatch * sb = const_cast<motor::StateManager *> (st)->getSpriteBatch();
	sb->begin();
	//level.draw(sb, tiles);
	
	sb->draw(*tiles, Rectangle(0, Window::getInstance()->getHeight() - 8*2, 8*4*2, 8*2), Rectangle(0,16,8,8));

	sb->draw(*tiles, Rectangle(player.position.x, player.position.y, 9, 8), Rectangle(0, 0, 9, 8), Vector4(1.0), 0, Vector2(4+1, 7+1), 4.0, 4);
	//sb->draw(*tiles, Rectangle(player.position.x, player.position.y, 9, 8), Rectangle(0, 0, 9, 8), Vector4(1.0), 0, Vector2(0, 0), 4.0, 4);
	for(Item & ent : items)
	{
		if(ent == LIFE_POTION)
		{
			sb->draw(*tiles, Rectangle(ent.position.x, ent.position.y, ent.hitbox.width, ent.hitbox.height), Rectangle(0, 9, 6, 6), Vector4(1.0), 0.0, Vector2(0, 0), 3.0, 3);
		}
		if(ent == MANA_POTION)
		{
			sb->draw(*tiles, Rectangle(ent.position.x, ent.position.y, ent.hitbox.width, ent.hitbox.height), Rectangle(7, 9, 6, 6), Vector4(1.0), 0.0, Vector2(0, 0), 3.0, 3);
		}
	}

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
