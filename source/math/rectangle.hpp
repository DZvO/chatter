#ifndef _RECTANGLE_HPP
#define _RECTANGLE_HPP

#include "lib/glm/glm.hpp"

class Rectangle
{
	public:
		float x, y;
		float width, height;
		Rectangle(const float & x, const float & y, const float & width, const float & height) : x(x), y(y), width(width), height(height)
		{};
};

#endif
