#ifndef _SPACESHIP_HPP
#define _SPACESHIP_HPP

#include "lib/glm/glm.hpp"
#include "math/vector.hpp"
#include "math/rectangle.hpp"
#include "graphics/window.hpp"
using namespace motor;

class Player
{
	public:
		Vector2 position;
		Vector2 velocity;
		Vector2 acceleration;
		Vector2 jump;
		Vector2 gravity;
		Vector2 direction;
		float rotation;
		float laser_cooldown;
		float jump_cooldown;
		Rectangle hitbox;
		bool flying;
		bool jumping;
		float jump_duration;

		Player();
		void update();
		Rectangle getHitbox ()
		{
			return Rectangle(position.x, position.y, hitbox.width, hitbox.height);
		}
};

#endif
