#include "level.hpp"

Level::Level(const Vector2 & dim)
{
	//blocks.reserve(dim.x * dim.y);
	//blocks = new unsigned char [ dim.x * dim.y ];
	blocks = new unsigned char [400*400];
	HEIGHT = dim.x;
	WIDTH = dim.y;

	for(int x = 0; x < WIDTH; x += 1)
	{
		for(int y = 0; y < HEIGHT; y += 1)
			blocks[x + y * WIDTH] = 0;
	}
}

void Level::load (std::string path)
{
}

void Level::draw (SpriteBatch * sb, Image * tiles)
{
	for(int x = 0; x < WIDTH; x += 1)
	{
		for(int y = 0; y < HEIGHT; y += 1)
		{
			//cout << "x: " << x << " y: " << y << '\n';
			if(blocks[x + y * WIDTH] != 0)
				sb->draw(*tiles, Rectangle(x*16, y*16, 8, 8), Rectangle(0, 16, 8, 8), Vector4(1), 0, Vector2(0), 2, 2);
		}
	}
}

void Level::add (const Vector2 & pos, unsigned char type)
{
	unsigned int index = (int)pos.x + ((int)pos.y * WIDTH);
	if(index >= WIDTH * HEIGHT)
	{
		//cout << "Bad access" << '\n';
		return;
	}
	blocks[index] = type;
}

unsigned char Level::get (const Vector2 & pos)
{
	unsigned int index = (int)pos.x + ((int)pos.y * WIDTH);
	if(index >= WIDTH * HEIGHT)
	{
		//cout << "Bad access" << '\n';
		return 0;
	}
	return blocks[index];
}

Rectangle Level::getBB (const Vector2 & pos)
{
	return Rectangle(((int)pos.x / 16) * 16, ((int)pos.y / 16) * 16, 16, 16);
}
