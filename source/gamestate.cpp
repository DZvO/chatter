#include "gamestate.hpp"
motor::state::GameState::GameState (): level(Vector2(100, 100))
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
	for(int x = 0; x < (int)(Window::getInstance()->getWidth() / 16); x++)
		level.add(Vector2(x, (int)(Window::getInstance()->getHeight() / 16)), 1);
	level.add(Vector2(8, (int)(Window::getInstance()->getHeight()/16)-1), 1);
	level.add(Vector2(9, (int)(Window::getInstance()->getHeight()/16)-1), 1);
	level.add(Vector2(10, (int)(Window::getInstance()->getHeight()/16)-1), 1);

	level.add(Vector2(12, (int)(Window::getInstance()->getHeight()/16)-1), 1);
	level.add(Vector2(13, (int)(Window::getInstance()->getHeight()/16)-1), 1);
	level.add(Vector2(14, (int)(Window::getInstance()->getHeight()/16)-1), 1);
	level.add(Vector2(15, (int)(Window::getInstance()->getHeight()/16)-1), 1);

	level.add(Vector2(17, (int)(Window::getInstance()->getHeight()/16)-2), 1);
	level.add(Vector2(18, (int)(Window::getInstance()->getHeight()/16)-2), 1);
	level.add(Vector2(19, (int)(Window::getInstance()->getHeight()/16)-2), 1);
	level.add(Vector2(20, (int)(Window::getInstance()->getHeight()/16)-2), 1);

	level.add(Vector2(21, (int)(Window::getInstance()->getHeight()/16)-3), 1);
	level.add(Vector2(22, (int)(Window::getInstance()->getHeight()/16)-3), 1);
	level.add(Vector2(23, (int)(Window::getInstance()->getHeight()/16)-3), 1);
	level.add(Vector2(24, (int)(Window::getInstance()->getHeight()/16)-3), 1);

	level.add(Vector2(25, (int)(Window::getInstance()->getHeight()/16)-4), 1);
	level.add(Vector2(26, (int)(Window::getInstance()->getHeight()/16)-4), 1);
	level.add(Vector2(27, (int)(Window::getInstance()->getHeight()/16)-4), 1);
	level.add(Vector2(28, (int)(Window::getInstance()->getHeight()/16)-4), 1);

	level.add(Vector2(29, (int)(Window::getInstance()->getHeight()/16)-5), 1);
	level.add(Vector2(30, (int)(Window::getInstance()->getHeight()/16)-5), 1);
	level.add(Vector2(31, (int)(Window::getInstance()->getHeight()/16)-5), 1);
	level.add(Vector2(32, (int)(Window::getInstance()->getHeight()/16)-5), 1);
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
	float xSpeed = 6.00, ySpeed = 8.0;
	if(input->isPressedSym("left") && input->isPressedSym("right"))
	{
		player.acceleration.x = 0;
		player.velocity.x = 0;
	}
	else if(input->isPressedSym("left") || input->isPressedSym("right"))
	{
		if(input->isPressedSym("left"))
		{
			if(player.velocity.x > 0)
				player.velocity.x = 0;
			if(player.acceleration.x > 0)
				player.acceleration.x = 0;

			if(player.acceleration.x > -8)
				player.acceleration.x -= xSpeed / (Window::getInstance()->getFrametime() / 10);
			player.direction.x = -1.0;
		}
		if(input->isPressedSym("right"))
		{
			if(player.velocity.x < 0)
				player.velocity.x = 0;
			if(player.acceleration.x < 0)
				player.acceleration.x = 0;

			if(player.acceleration.x < 8)
				player.acceleration.x += xSpeed / (Window::getInstance()->getFrametime() / 10);
			player.direction.x = +1.0;
		}
	}
	else
	{
		if(glm::abs(player.acceleration.x) < 0.1)
			player.acceleration.x = 0;
		else
			player.acceleration.x *= 0.6;
	}
	if(input->isPressedSym("up") && player.flying == false && player.jump_cooldown <= 0.0)
	{
		player.velocity.y = -6.2;
		player.jump.y = -(0.6);//* (Window::getInstance()->getFrametime() / 10);
		player.direction.y = -1;// * (Window::getInstance()->getFrametime());
		player.jump_cooldown = 0.5f;
		player.jumping = true;
		cout << "[" << Window::getInstance()->getElapsedTime() << "] " << "jump" << "\n";
	}
	else if(!input->isPressedSym("up"))// || player.jump_duration > 0.2)
	{
		player.jump.y = 0;
		player.jump_duration = 0;
		player.jumping = false;
	}
	if(input->isPressedSym("down"))
	{
		player.velocity.y += ySpeed * (Window::getInstance()->getFrametime() / 10);
		player.direction.y = +1;// * (Window::getInstance()->getFrametime());
	}

	player.update();

	//collision detection
	if(true)
	{
		Rectangle staticGeom = Rectangle(0, Window::getInstance()->getHeight() - 8*2, 8*4*2, 8*2);
		Rectangle prevPlayerHitbox = prev;
		prevPlayerHitbox.width *= 4.0;
		prevPlayerHitbox.x += 6.0*4.0;
		prevPlayerHitbox.width -= 6.0*4.0 + 5.0*4.0;
		prevPlayerHitbox.height *= 4.0;

		Rectangle currPlayerHitbox = player.getHitbox();
		currPlayerHitbox.width *= 4.0;
		currPlayerHitbox.x += 6.0*4.0;
		currPlayerHitbox.width -= 6.0*4.0 + 5.0*4.0;
		currPlayerHitbox.height *= 4.0;

		//--------------------------------------------------------
		bool collidedLeft = false;
		bool collidedRight = false;
		bool solidBlockInRange = false;
		float newYLeft = 0.0;
		float newYRight = 0.0;
		int directionX = 0;
		if(prevPlayerHitbox.getCenter().x < currPlayerHitbox.getCenter().x)
			directionX = +1;
		else
			directionX = -1;

		for(int y = (int)prevPlayerHitbox.getLowerLeft().y;		//don't use a direction variable here because
				y <= (int)currPlayerHitbox.getLowerLeft().y; y++) //this automatically skips checking while in air
		{
			for(int x = (int)prevPlayerHitbox.getLowerLeft().x;
					x != (int)currPlayerHitbox.getLowerLeft().x+directionX; x += directionX)
			{
				if(level.get(Vector2(x/16,y/16)) == 0)
					continue;
				solidBlockInRange = true;

				staticGeom = level.getBB(Vector2(x,y));
				if(staticGeom.intersectslineTop(
							Line(prevPlayerHitbox.getLowerLeft(), currPlayerHitbox.getLowerLeft()))// ||
						//staticGeom.collides(currPlayerHitbox)
					)
				{
					if(player.velocity.y > 0.0)
					{
						if(newYLeft == 0)
							newYLeft = staticGeom.y - player.hitbox.height*4;
						if((staticGeom.y - player.hitbox.height*4) > newYLeft)
							newYLeft = staticGeom.y - player.hitbox.height*4;

						//cout << "[" << Window::getInstance()->getElapsedTime() << "] " << "collides y on the left" << '\n';
						collidedLeft = true;
					}
				}
			}
		}
		//--------------------------------------------------------
		for(int y = (int)prevPlayerHitbox.getLowerRight().y;
				y <= (int)currPlayerHitbox.getLowerRight().y; y++)
		{
			for(int x = (int)prevPlayerHitbox.getLowerRight().x;
					x != (int)currPlayerHitbox.getLowerRight().x+directionX; x += directionX)
			{
				if(level.get(Vector2(x/16,y/16)) == 0)
					continue;
				solidBlockInRange = true;

				staticGeom = level.getBB(Vector2(x,y));
				if(staticGeom.intersectslineTop(
							Line(prevPlayerHitbox.getLowerRight(), currPlayerHitbox.getLowerRight()))// ||
						//staticGeom.collides(currPlayerHitbox)
					)
				{
					if(player.velocity.y > 0.0)
					{
						if(newYRight == 0)
							newYRight = staticGeom.y - player.hitbox.height*4;
						if((staticGeom.y - player.hitbox.height*4) > newYRight)
							newYRight = staticGeom.y - player.hitbox.height*4;

						//cout << "[" << Window::getInstance()->getElapsedTime() << "] " << "collides y on the right" << '\n';
						collidedRight = true;
					}
				}
			}
		}

		int directionY = 0;
		if(prevPlayerHitbox.getCenter().y < currPlayerHitbox.getCenter().y)
			directionY = +1;
		else
			directionY = -1;

		float newXRight = 0;

		
		for(int y = (int)prevPlayerHitbox.getLowerRight().y-1;
				y <= (int)currPlayerHitbox.getLowerRight().y-1; y++)
		{
			for(int x = (int)prevPlayerHitbox.getLowerRight().x;
					x != (int)currPlayerHitbox.getLowerRight().x+directionX; x+=directionX)
			{
				if(level.get(Vector2(x/16,y/16)) == 0)
					continue;

				staticGeom = level.getBB(Vector2(x,y));
				if(staticGeom.intersectslineLeft(
							Line(prevPlayerHitbox.getLowerRight(), currPlayerHitbox.getLowerRight()))
					)
				{
					if(newXRight == 0 || (staticGeom.x - player.getHitbox().width*4) < newXRight)
						newXRight = staticGeom.x + 16 - player.getHitbox().width*4;
					cout << "collide x\n";
				}
			}
		}
		if(newXRight != 0.0)
		{
			player.position.x = newXRight;
			player.velocity.x = 0;
			player.acceleration.x = 0;
		}
		//--------------------------------------------------------
		if(collidedLeft || collidedRight)
		{
			player.velocity.y = 0;
			player.gravity.y = 0;
			player.acceleration.y = 0;
			player.jump.y = 0;
			player.position.y = (newYLeft < newYRight ? newYRight : newYLeft); //use the value that is smaller/is higher
			player.flying = false;
		}
		else
		{
			if(!solidBlockInRange)
			{
				player.gravity.y = 1;
				player.flying = true;
				//cout << "enabled gravity!" << '\n';
			}
		}
		//--------------------------------------------------------

		/*
			 staticGeom = Rectangle(0, 0, 5, 600);
			 if(staticGeom.intersectslineRight(Line(prevPlayerHitbox.getLowerLeft(), currPlayerHitbox.getLowerLeft())) ||
			 staticGeom.intersectslineRight(Line(prevPlayerHitbox.getUpperLeft(), currPlayerHitbox.getUpperLeft())) ||
			 staticGeom.intersectslineRight(Line(prevPlayerHitbox.getCenter(), currPlayerHitbox.getCenter())))
		//staticGeom.collides(currPlayerHitbox))
		{
		if(player.velocity.x < 0.0)
		{
		player.velocity.x = 0;
		player.acceleration.x = 0;
		player.position.x = staticGeom.x + staticGeom.width;// - (player.hitbox.width * 4);
		cout << "[" << Window::getInstance()->getElapsedTime() << "] " << "xcollides" << '\n';
		}
		}
		else
		{
		}
		*/
	}

	//if(input->isPressedSym(Input::kEnter) && player.laser_cooldown <= 0.0)
	//{
	//shootLazorBEAM(player);
	//}

	/*for(auto & blt : bullets)
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
		}*/
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
	unsigned int switchTime = 6;
	//int yCoord = 213;
	int yCoord = 230;

	playerTexRect = Rectangle(18, yCoord, 16, 16);
	//messy animation code :3
	if(player.direction.x == 1 && player.velocity.x > 0.5)
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
	else if(player.direction.x == -1 && player.velocity.x < -0.5)
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
	
	/*
	Rectangle phitb = Rectangle(player.getHitbox().x, player.getHitbox().y, player.getHitbox().width*4, player.getHitbox().height*4);
	sb->draw(*tiles, Rectangle(
				(((int)phitb.getLowerLeft().x/16)) *16,
				(((int)phitb.getLowerLeft().y/16)) *16,
				16, 16), Rectangle(0,0,8,8), Vector4(1), 0, Vector2(0), 1, 10);
	sb->draw(*tiles, Rectangle(
				(((int)(phitb.getLowerRight().x)/16)) *16,
				(((int)phitb.getLowerRight().y/16)) *16,
				16, 16), Rectangle(8,0,8,8), Vector4(1), 0, Vector2(0), 1, 10);

	sb->draw(*tiles, Rectangle(
				(((int)(phitb.getUpperRight().x)/16)) *16,
				(((int)phitb.getUpperRight().y/16)) *16,
				16, 16), Rectangle(16,0,8,8), Vector4(1), 0, Vector2(0), 1, 10);
	sb->draw(*tiles, Rectangle(
				(((int)(phitb.getUpperLeft().x)/16)) *16,
				(((int)phitb.getUpperLeft().y/16)) *16,
				16, 16), Rectangle(24,0,8,8), Vector4(1), 0, Vector2(0), 1, 10);
				*/

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
