#ifndef _ENEMY_HPP
#define _ENEMY_HPP

#include <iostream>
#include "math/vector.hpp"
#include "math/rectangle.hpp"

class Enemy
{
	int life;
	Vector2 position;
	Rectangle hitbox;

};
#endif
