#ifndef _RECTANGLE_HPP
#define _RECTANGLE_HPP

#include <iostream>
#include "lib/glm/glm.hpp"

class Rectangle
{
	public:
		float x, y;
		float width, height;
		Rectangle(const float & x, const float & y, const float & width, const float & height) : x(x), y(y), width(width), height(height)
		{};
		Rectangle(): x(0), y(0), width(0), height(0)
		{};
		bool isInside(float x, float y)
		{
			if(
					(this->x <= x) && (this->x + this->width >= x)
				&&	
					(this->y <= y) && (this->y + this->height >= y)
				)
			{
				return true;
			}
			return false;
		}

		bool collides (const Rectangle & r)
		{
			if(	isInside(r.x, r.y) ||
					isInside(r.x + r.width, r.y) ||
					isInside(r.x, r.y + r.height) ||
					isInside(r.x + r.width, r.y + r.height))
				return true;
			return false;
		}

		friend std::ostream & operator << (std::ostream & os, const Rectangle & r)
		{
			os << "R(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << ")";
			return os;
		}
};

#endif
