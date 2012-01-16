#include "level.hpp"

Level::Level(const Vector2 & dim)
{
	//blocks.reserve(dim.x * dim.y);
	//blocks = new unsigned char [ dim.x * dim.y ];
	blocks = new unsigned char [800*600];
	HEIGHT = dim.x;
	WIDTH = dim.y;
}

void Level::load (std::string path)
{
}

void Level::draw (SpriteBatch * sb, Image * tiles)
{
	for(int x = 0; x < WIDTH; x += 8)
	{
		for(int y = 0; y < HEIGHT; y += 8)
		{
			//cout << "x: " << x << " y: " << y << '\n';
			sb->draw(*tiles, Rectangle(x, y, 8, 8), Rectangle(0, 16, 8, 8), Vector4(1), 0, Vector2(0), 1, 2);
		}
	}
}

void Level::add (const Vector2 & pos, unsigned char type)
{
	unsigned int index = (int)pos.x + ((int)pos.y * WIDTH);
	if(index >= WIDTH * HEIGHT)
	{
		cout << " TT " << '\n';
	}
	blocks[index] = type;
}

unsigned char Level::get (const Vector2 & pos)
{
	unsigned int index = (int)pos.x + ((int)pos.y * WIDTH);
	if(index >= WIDTH * HEIGHT)
	{
		cout << " TT " << '\n';
	}
	return blocks[index];
}

Rectangle Level::getBB (const Vector2 & pos)
{
	return Rectangle((int)pos.x, (int)pos.y, 1, 1);
}
