#ifndef _RECTANGLE_HPP
#define _RECTANGLE_HPP

#include <iostream>
#include "lib/glm/glm.hpp"
#include "math/vector.hpp"

class Rectangle
{
	public:
		float x, y;
		float width, height;
		Rectangle(const float & x, const float & y, const float & width, const float & height) : x(x), y(y), width(width), height(height)
		{};
		Rectangle(): x(0), y(0), width(0), height(0)
		{};
		bool isInside(float x, float y) const
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

		bool collides (const Rectangle & r) const
		{
			if(	isInside(r.x, r.y) ||
					isInside(r.x + r.width, r.y) ||
					isInside(r.x, r.y + r.height) ||
					isInside(r.x + r.width, r.y + r.height))
				return true;
			return false;
		}

		bool intersectsline (const Vector2 & a, const Vector2 & b) const
		{
			if
				(
				 ((x > b.x && x > a.x) || (y > b.y && y > a.y))
				 ||
				 ((x+width < a.x && x+width < b.x) || (y+height < a.y && y+height < b.y))
				)
			{
				return false;
			}
			return true;
		}

		Vector2 getCenter ()
		{
			return Vector2(x + width/2, y + height/2);
		}

		bool operator== (const Rectangle & r)
		{
			if(x == r.x && y == r.y && width == r.width && height == r.height)
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
