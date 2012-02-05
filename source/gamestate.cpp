#include "gamestate.hpp"
motor::state::GameState::GameState (): level(Vector2(100, 100))
{
	background = new Image("data/background.png");
	tiles = new Image("data/tiles.png");
	chars = new Image("data/char.png");
	player.position = Vector2(20, 200);
	player.hitbox.width = 16;
	player.hitbox.height = 16;
	playerScale = 3;

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


	level.add(Vector2(17, (int)(Window::getInstance()->getHeight()/16)-2), 1);
	level.add(Vector2(18, (int)(Window::getInstance()->getHeight()/16)-2), 1);
	level.add(Vector2(19, (int)(Window::getInstance()->getHeight()/16)-2), 1);
	level.add(Vector2(20, (int)(Window::getInstance()->getHeight()/16)-2), 1);

	level.add(Vector2(24, (int)(Window::getInstance()->getHeight()/16)-3), 1);
	level.add(Vector2(25, (int)(Window::getInstance()->getHeight()/16)-3), 1);
	level.add(Vector2(26, (int)(Window::getInstance()->getHeight()/16)-3), 1);
	level.add(Vector2(27, (int)(Window::getInstance()->getHeight()/16)-3), 1);

	/*level.add(Vector2(21, (int)(Window::getInstance()->getHeight()/16)-3), 1);
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
	level.add(Vector2(32, (int)(Window::getInstance()->getHeight()/16)-5), 1);*/
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
	//if(input->isPressedSym("down"))
	//{
		//player.velocity.y += ySpeed * (Window::getInstance()->getFrametime() / 10);
		//player.direction.y = +1;// * (Window::getInstance()->getFrametime());
	//}

	player.update();

	//collision detection
	if(true)
	{
		Rectangle staticGeom = Rectangle(0,0,0,0);
		Rectangle prevPlayerHitbox = prev;
		prevPlayerHitbox.width *= playerScale;
		prevPlayerHitbox.x += 7.0*playerScale;;
		prevPlayerHitbox.width -= 7.0*playerScale + 6.0*playerScale;
		prevPlayerHitbox.height *= playerScale;

		Rectangle currPlayerHitbox = player.getHitbox();
		currPlayerHitbox.width *= playerScale;
		currPlayerHitbox.x += 7.0*playerScale;
		currPlayerHitbox.width -= 7.0*playerScale + 6.0*playerScale;
		currPlayerHitbox.height *= playerScale;

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
							Line(prevPlayerHitbox.getLowerLeft(), currPlayerHitbox.getLowerLeft()))
					)
				{
					if(player.velocity.y > 0.0)
					{
						if(newYLeft == 0)
							newYLeft = staticGeom.y - player.hitbox.height*playerScale;
						if((staticGeom.y - player.hitbox.height*playerScale) > newYLeft)
							newYLeft = staticGeom.y - player.hitbox.height*playerScale;
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
							Line(prevPlayerHitbox.getLowerRight(), currPlayerHitbox.getLowerRight()))
					)
				{
					if(player.velocity.y > 0.0)
					{
						if(newYRight == 0)
							newYRight = staticGeom.y - player.hitbox.height*playerScale;
						if((staticGeom.y - player.hitbox.height*playerScale) > newYRight)
							newYRight = staticGeom.y - player.hitbox.height*playerScale;
						collidedRight = true;
					}
				}
			}
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

		if(directionX > 0) //going right
		{
			cout << "going right!\n";
			for(int x = (int)prevPlayerHitbox.getLowerRight().x;
					x <= (int)currPlayerHitbox.getLowerRight().x+16; x++)
			{
				if(level.get(Vector2(x/16, ((int)currPlayerHitbox.getLowerRight().y)/16-1)) == 0)
					continue;
				//checked_red.push_back(Rectangle(((int)x / 16) * 16, ((int)prevPlayerHitbox.getLowerRight().y/16-1) * 16, 16, 16));
				Rectangle bbRight = 
					Rectangle(((int)x/16)*16, ((int)prevPlayerHitbox.getLowerRight().y/16-1)*16, 16, 16);
				//checked_red.push_back(bbRight);
				if(bbRight.intersectslineLeft(
							Line(
								prevPlayerHitbox.getLowerRight(), 
								currPlayerHitbox.getLowerRight())))
				{
					if(player.velocity.x > 0)
					{
						player.velocity.x = 0;
						player.acceleration.x = 0;
						player.position.x = bbRight.x - (7*playerScale + currPlayerHitbox.width);
					}
					break;
				}
			}
			prevPlayerHitbox.x += 1*playerScale;
			currPlayerHitbox.x += 1*playerScale;
			for(int x = (int)prevPlayerHitbox.getLowerRight().x;
					x <= (int)currPlayerHitbox.getLowerRight().x+16; x++)
			{
				if(level.get(Vector2(x/16, ((int)currPlayerHitbox.getLowerRight().y)/16-2)) == 0)
					continue;
				//checked_red.push_back(Rectangle(((int)x / 16) * 16, ((int)prevPlayerHitbox.getLowerRight().y/16-1) * 16, 16, 16));
				Rectangle bbRight = 
					Rectangle(((int)x/16)*16, ((int)prevPlayerHitbox.getLowerRight().y/16-2)*16, 16, 16);
				checked_red.push_back(bbRight);
				if(bbRight.intersectslineLeft(
							Line(
								Vector2(prevPlayerHitbox.getLowerRight().x, prevPlayerHitbox.getLowerRight().y - 8*playerScale),
								Vector2(currPlayerHitbox.getLowerRight().x, prevPlayerHitbox.getLowerRight().y - 8*playerScale))
							))
				{
					if(player.velocity.x > 0)
					{
						player.velocity.x = 0;
						player.acceleration.x = 0;
						player.position.x = bbRight.x - (7*playerScale + currPlayerHitbox.width) - 1*playerScale; //the 1 pixel is for the head
					}
					break;
				}
			}
			for(int x = (int)prevPlayerHitbox.getLowerRight().x;
					x <= (int)currPlayerHitbox.getLowerRight().x+16; x++)
			{
				if(level.get(Vector2(x/16, ((int)currPlayerHitbox.getLowerRight().y)/16-3)) == 0)
					continue;
				//checked_red.push_back(Rectangle(((int)x / 16) * 16, ((int)prevPlayerHitbox.getLowerRight().y/16-1) * 16, 16, 16));
				Rectangle bbRight = 
					Rectangle(((int)x/16)*16, ((int)prevPlayerHitbox.getLowerRight().y/16-3)*16, 16, 16);
				checked_red.push_back(bbRight);
				if(bbRight.intersectslineLeft(
							Line(
								Vector2(prevPlayerHitbox.getLowerRight().x, prevPlayerHitbox.getLowerRight().y - 12*playerScale),
								Vector2(currPlayerHitbox.getLowerRight().x, prevPlayerHitbox.getLowerRight().y - 12*playerScale))
							))
				{
					if(player.velocity.x > 0)
					{
						player.velocity.x = 0;
						player.acceleration.x = 0;
						player.position.x = bbRight.x - (7*playerScale + currPlayerHitbox.width) - 1*playerScale; //the 1 pixel is for the head
					}
					break;
				}
			}
		}
		else if(directionX < 0) //going left
		{
			cout << "going left!\n";
			for(int x = (int)prevPlayerHitbox.getLowerLeft().x;
					x >= (int)currPlayerHitbox.getLowerLeft().x-16; x--)
			{
				if(level.get(Vector2(x/16, ((int)currPlayerHitbox.getLowerLeft().y)/16-1)) == 0)
					continue;
				//checked_red.push_back(Rectangle(((int)x / 16) * 16, ((int)prevPlayerHitbox.getLowerRight().y/16-1) * 16, 16, 16));
				Rectangle bbLeft = 
					Rectangle(((int)x/16)*16, ((int)prevPlayerHitbox.getLowerLeft().y/16-1)*16, 16, 16);
				checked_red.push_back(bbLeft);
				if(bbLeft.intersectslineRight(
							Line(
								prevPlayerHitbox.getLowerLeft(), 
								currPlayerHitbox.getLowerLeft())))
				{
					if(player.velocity.x < 0)
					{
						player.velocity.x = 0;
						player.acceleration.x = 0;
						player.position.x = bbLeft.x + bbLeft.width - 7*playerScale;
					}
					break;
				}
			}
			prevPlayerHitbox.x -= 1*playerScale;
			currPlayerHitbox.x -= 1*playerScale;
			for(int x = (int)prevPlayerHitbox.getLowerLeft().x;
					x >= (int)currPlayerHitbox.getLowerLeft().x-16; x--)
			{
				if(level.get(Vector2(x/16, ((int)currPlayerHitbox.getLowerLeft().y)/16-2)) == 0)
					continue;
				//checked_red.push_back(Rectangle(((int)x / 16) * 16, ((int)prevPlayerHitbox.getLowerRight().y/16-1) * 16, 16, 16));
				Rectangle bbLeft = 
					Rectangle(((int)x/16)*16, ((int)prevPlayerHitbox.getLowerLeft().y/16-2)*16, 16, 16);
				checked_red.push_back(bbLeft);
				if(bbLeft.intersectslineRight(
							Line(
								Vector2(prevPlayerHitbox.getLowerLeft().x, prevPlayerHitbox.getLowerLeft().y - 8*playerScale),
								Vector2(currPlayerHitbox.getLowerLeft().x, prevPlayerHitbox.getLowerLeft().y - 8*playerScale))
							))
				{
					if(player.velocity.x < 0)
					{
						player.velocity.x = 0;
						player.acceleration.x = 0;
						player.position.x = bbLeft.x + bbLeft.width - 7*playerScale + 1*playerScale;
					}
					break;
				}
			}
			for(int x = (int)prevPlayerHitbox.getLowerLeft().x;
					x >= (int)currPlayerHitbox.getLowerLeft().x-16; x--)
			{
				if(level.get(Vector2(x/16, ((int)currPlayerHitbox.getLowerLeft().y)/16-3)) == 0)
					continue;
				//checked_red.push_back(Rectangle(((int)x / 16) * 16, ((int)prevPlayerHitbox.getLowerRight().y/16-1) * 16, 16, 16));
				Rectangle bbLeft = 
					Rectangle(((int)x/16)*16, ((int)prevPlayerHitbox.getLowerLeft().y/16-3)*16, 16, 16);
				checked_red.push_back(bbLeft);
				if(bbLeft.intersectslineRight(
							Line(
								Vector2(prevPlayerHitbox.getLowerLeft().x, prevPlayerHitbox.getLowerLeft().y - 12*playerScale),
								Vector2(currPlayerHitbox.getLowerLeft().x, prevPlayerHitbox.getLowerLeft().y - 12*playerScale))
							))
				{
					if(player.velocity.x < 0)
					{
						player.velocity.x = 0;
						player.acceleration.x = 0;
						player.position.x = bbLeft.x + bbLeft.width - 7*playerScale + 1*playerScale;
					}
					break;
				}
			}
		}
	}

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

	sb->draw(*chars, Rectangle(player.position.x, player.position.y, 16, 16), playerTexRect, Vector4(1), 0, Vector2(0), playerScale, 4);

	Rectangle currPlayerHitbox = player.getHitbox();
	currPlayerHitbox.width *= playerScale;
	currPlayerHitbox.x += 7.0*playerScale;
	currPlayerHitbox.width -= 7.0*playerScale + 6.0*playerScale;
	currPlayerHitbox.height *= playerScale;

	//Rectangle phitb = Rectangle(player.getHitbox().x, player.getHitbox().y, player.getHitbox().width*4, player.getHitbox().height*4);
	Rectangle phitb = currPlayerHitbox;
	for(auto & rec : checked_red)
	{
		sb->draw(*tiles, rec, Rectangle(0,0,8,8), Vector4(1), 0, Vector2(0), 1, 10);
	}
	checked_red.clear();
	//sb->draw(*tiles, Rectangle(
	//(((int)phitb.getLowerRight().x/16)) *16,
	//(((int)phitb.getLowerRight().y/16)-1) *16,
	//16, 16), Rectangle(0,0,8,8), Vector4(1), 0, Vector2(0), 1, 10);
	/*sb->draw(*tiles, Rectangle(
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
