#ifndef _LEVEL_HPP
#define _LEVEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include "graphics/image.hpp"
#include "graphics/spritebatch.hpp"
#include "math/vector.hpp"
#include "math/rectangle.hpp"

class Level
{
	public:
		Level(const Vector2 & dimensions);
		void load (std::string path);
		void draw (SpriteBatch * sb, Image * tiles);

		void add (const Vector2 & pos, unsigned char type);
		unsigned char get (const Vector2 & pos);
		Rectangle getBB (const Vector2 & pos);

	private:
		//std::vector<unsigned char> blocks;
		unsigned char * blocks;
		unsigned int HEIGHT, WIDTH;
};

#endif
