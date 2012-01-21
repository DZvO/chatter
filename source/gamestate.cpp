#include "gamestate.hpp"
motor::state::GameState::GameState (): level(Vector2(400, 400))
{
	background = new Image("data/background.png");
	tiles = new Image("data/tiles.png");
	chars = new Image("data/char.png");
	player.position = Vector2(20, 200);
	player.hitbox.width = 16;
	player.hitbox.height = 16;

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
	level.add(Vector2(0,0), 1);
	level.add(Vector2(2,0), 1);
	level.add(Vector2(4,0), 1);

	level.add(Vector2(0, (int)(Window::getInstance()->getHeight() / 16)), 1);
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
		if(input->isPressed(Input::kBackspace))
		{
			player.position = Vector2(100, 200);
		}
	}

	Rectangle prev = player.getHitbox();
	player.update();
	float xSpeed = 1.00, ySpeed = 8.0;
	if(input->isPressedSym("left"))
	{
		player.velocity.x -= xSpeed * (Window::getInstance()->getFrametime() / 10);
		player.direction.x = -1.0;
	}
	if(input->isPressedSym("right"))
	{
		player.velocity.x += xSpeed * (Window::getInstance()->getFrametime() / 10);
		player.direction.x = +1.0;
	}
	if(input->isPressedSym("up") && player.flying == false && player.jump_cooldown <= 0.0)
	{
		player.velocity.y -= ySpeed * 1.2;//* (Window::getInstance()->getFrametime() / 10);
		player.direction.y -= 10.0;// * (Window::getInstance()->getFrametime());
		player.jump_cooldown = 0.5f;
		//cout << "jump" << "\n";
	}
	if(input->isPressedSym("down"))
	{
		player.velocity.y += ySpeed * (Window::getInstance()->getFrametime() / 10);
		player.direction.y += 10.0;// * (Window::getInstance()->getFrametime());
	}
	

	//collision detection
	{
		Rectangle staticGeom = Rectangle(0, Window::getInstance()->getHeight() - 8*2, 8*4*2*10, 8*2);
		Rectangle prevPlayer = prev;
		prevPlayer.width *= 4.0;
		prevPlayer.height *= 4.0;
		//prevPlayer.x += (7*4) + (0.5*4);
		//prevPlayer.width -= (5*4) + (0.5*4);
		Rectangle currPlayer = player.getHitbox();
		currPlayer.width *= 4.0;
		currPlayer.height *= 4.0;
		//currPlayer.x += (7*4) + (0.5*4);
		//currPlayer.width -= (5*4) + (0.5*4);

		//clipping
		//TODO only do collision for top face of staticGeom, so the player cannot jump while the players lower face touches the sides -> else there would be some kind of wall jumping
		if(
				(staticGeom.intersectslineTop(prevPlayer.getLowerLeft(), currPlayer.getLowerLeft()) ||
				 staticGeom.intersectslineTop(prevPlayer.getLowerRight(), currPlayer.getLowerRight()) ||
				 staticGeom.intersectslineTop(prevPlayer.getCenter(), currPlayer.getCenter()))
			)
		{
			if(player.velocity.y > 0.0)
			{
				player.velocity.y = 0;
				player.position.y = staticGeom.y - player.hitbox.height*4;
				player.flying = false;
				cout << "collides" << '\n';
			}
		}
		//flying
		else
		{
			if(currPlayer.y != (staticGeom.y - player.hitbox.height*4))
			{
				if(player.velocity.y < 7.0)
					player.velocity.y += 0.80;
					//player.velocity.y += 0.4;
				player.flying = true;
			}
		}

		staticGeom = Rectangle(0, 0, 5, 600);
		if(staticGeom.intersectslineRight(prevPlayer.getLowerLeft(), currPlayer.getLowerLeft()) ||
				staticGeom.intersectslineRight(prevPlayer.getUpperLeft(), currPlayer.getUpperLeft()) ||
				staticGeom.intersectslineRight(prevPlayer.getCenter(), currPlayer.getCenter()))
			//staticGeom.collides(currPlayer))
		{
			if(player.velocity.x < 0.0)
			{
				player.velocity.x = 0;
				player.position.x = staticGeom.x + staticGeom.width;// - (player.hitbox.width * 4);
				cout << "xcollides" << '\n';
			}
		}
		else
		{
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
	level.draw(sb, tiles);
	//

	ticks++;
	Rectangle playerTexRect;
	int switchTime = 6;
	//int yCoord = 213;
	int yCoord = 230;

	//messy animation code :3
	if(player.velocity.x > 0.5)
	{
		if(player.velocity.y != 0)
		{
			if(player.velocity.y < -2.2)
				playerTexRect = Rectangle(1, 264, 16, 16);
			else if(player.velocity.y > 2.2)
				playerTexRect = Rectangle(35, 264, 16, 16);
			else
				playerTexRect = Rectangle(18, 264, 16, 16);
		}
		else
		{
			if(ticks < switchTime)
				playerTexRect = Rectangle(18, yCoord, 16, 16);
			else if (ticks < switchTime*2)
				playerTexRect = Rectangle(35, yCoord, 16, 16);
			else if(ticks < switchTime*3)
				playerTexRect = Rectangle(52, yCoord, 16, 16);
			else if (ticks < switchTime*4)
				playerTexRect = Rectangle(69, yCoord, 16, 16);
			else
			{
				ticks = 0;
				playerTexRect = Rectangle(18, yCoord, 16, 16);
			}
		}
	}
	else if(player.velocity.x < -0.5)
	{
		if(player.velocity.y != 0)
		{
			if(player.velocity.y < -2.2)
				playerTexRect = Rectangle(155, 264, 16, 16);
			else if(player.velocity.y > 2.2)
				playerTexRect = Rectangle(121, 264, 16, 16);
			else
				playerTexRect = Rectangle(138, 264, 16, 16);
		}
		else
		{
			if(ticks < switchTime)
				playerTexRect = Rectangle(138, yCoord, 16, 16);
			else if (ticks < switchTime*2)
				playerTexRect = Rectangle(121, yCoord, 16, 16);
			else if(ticks < switchTime*3)
				playerTexRect = Rectangle(104, yCoord, 16, 16);
			else if (ticks < switchTime*4)
				playerTexRect = Rectangle(87, yCoord, 16, 16);
			else
			{
				ticks = 0;
				playerTexRect = Rectangle(138, yCoord, 16, 16);
			}
		}
	}
	else
	{
		if(player.direction.x > 0) //looking to the right
		{
			if(player.velocity.y != 0)
			{
				if(player.velocity.y < -2.2)
					playerTexRect = Rectangle(1, 264, 16, 16);
				else if(player.velocity.y > 2.2)
					playerTexRect = Rectangle(35, 264, 16, 16);
				else
					playerTexRect = Rectangle(18, 264, 16, 16);
			}
			else
				playerTexRect = Rectangle(1, yCoord, 16, 16);
		}
		else // looking to the left
		{
			if(player.velocity.y != 0)
			{
				if(player.velocity.y < -2.2)
					playerTexRect = Rectangle(155, 264, 16, 16);
				else if(player.velocity.y > 2.2)
					playerTexRect = Rectangle(121, 264, 16, 16);
				else
					playerTexRect = Rectangle(138, 264, 16, 16);
			}
			else
				playerTexRect = Rectangle(155, yCoord, 16, 16);
		}
	}

	sb->draw(*chars, Rectangle(player.position.x, player.position.y, 16, 16), playerTexRect, Vector4(1), 0, Vector2(0), 4, 4);

	//sb->draw(*tiles, Rectangle(0, Window::getInstance()->getHeight() - 8*2, 8*4*2*10, 8*2), Rectangle(0,16,8,8));
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
