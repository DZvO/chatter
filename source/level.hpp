#ifndef _LEVEL_HPP
#define _LEVEL_HPP

#include <iostream>
#include <string>
#include "graphics/spritebatch.hpp"

class Level
{
	public:
		Level();
		void load (std::string path);
		void draw (SpriteBatch * sb);
	private:
		std::vector<Block> blocks;
}

#endif
