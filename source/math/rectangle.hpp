#ifndef _RECTANGLE_HPP
#define _RECTANGLE_HPP

#include <iostream>
#include "lib/glm/glm.hpp"
#include "math/vector.hpp"
#include "math/line.hpp"

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
					(this->x < x) && (this->x + this->width > x)
				&&	
					(this->y < y) && (this->y + this->height > y)
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
				 ((x > b.x && x > a.x) ||	(y > b.y && y > a.y))
				 or
				 ((x+width < a.x && x+width < b.x) ||	(y+height < a.y && y+height < b.y))
				)
				return false;
			return true;
		}

		bool intersectslineTop (const Line & line)
		{
			return Line::intersects(Line(getUpperLeft(), getUpperRight()), line);

		}

		bool intersectslineBottom (const Line & line)
		{
			return Line::intersects(Line(getLowerLeft(), getLowerRight()), line);
		}
		
		bool intersectslineRight (const Line & line)
		{
			return Line::intersects(Line(getUpperRight(), getLowerRight()), line);
		}

		bool intersectslineLeft (const Line & line)
		{
			return Line::intersects(Line(getUpperLeft(), getLowerLeft()), line);
		}

		bool intersectsline (const Line & line) const
		{
			return (intersectsline(line.a, line.b));
		}

		Vector2 getCenter ()
		{
			return Vector2(x + width/2, y + height/2);
		}

		Vector2 getUpperLeft ()
		{
			return Vector2(x,y);
		}

		Vector2 getUpperRight ()
		{
			return Vector2(x+width, y);
		}

		Vector2 getLowerLeft ()
		{
			return Vector2(x, y+height);
		}

		Vector2 getLowerRight ()
		{
			return Vector2(x+width, y+height);
		}

		Line getTop ()
		{
			return Line(Vector2(x,y), Vector2(x+width, y));
		}

		Line getBottom ()
		{
			return Line(Vector2(x,y+height), Vector2(x+width,y+height));
		}

		Line getLeft ()
		{
			return Line(Vector2(x,y), Vector2(x,y+height));
		}

		Line getRight ()
		{
			return Line(Vector2(x+width,y), Vector2(x+width,y+height));
		}

		bool operator== (const Rectangle & r)
		{
			if(x == r.x && y == r.y && width == r.width && height == r.height)
				return true;
			return false;
		}

		friend std::ostream & operator << (std::ostream & os, const Rectangle & r)
		{
			os << "Rect(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << ")";
			return os;
		}
};

/*
class AABB //the same as rectangle, but the x,y coordinates are now the center point instead of the upper left corner
{
	public:
		float x, y;
		float width, height;
		AABB(const float & x, const float & y, const float & width, const float & height) : x(x), y(y), width(width), height(height)
		{};
		AABB(): x(0), y(0), width(0), height(0)
		{};

		bool isInside(float x, float y) const
		{
			//TODO
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
			//TODO
			if(	isInside(r.x, r.y) ||
					isInside(r.x + r.width, r.y) ||
					isInside(r.x, r.y + r.height) ||
					isInside(r.x + r.width, r.y + r.height))
				return true;
			return false;
		}

		bool intersectsline (const Vector2 & a, const Vector2 & b) const
		{
			//TODO
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
			//TODO
			return Vector2(x + width/2, y + height/2);
		}

		bool operator== (const Rectangle & r)
		{
			//TODO
			if(x == r.x && y == r.y && width == r.width && height == r.height)
				return true;
			return false;
		}

		friend std::ostream & operator << (std::ostream & os, const Rectangle & r)
		{
			//TODO
			os << "R(" << r.x << ", " << r.y << ", " << r.width << ", " << r.height << ")";
			return os;
		}
};
*/

#endif
