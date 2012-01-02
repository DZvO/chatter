#include "spaceship.hpp"
void SpaceShip::update ()
{
	//velocity = (acceleration * Vector2(Window::getInstance()->getFrametime() / 10)) + velocity;
	//position = (Vector2(0.5) * acceleration * (Vector2(Window::getInstance()->getFrametime() / 10) * Vector2(Window::getInstance()->getFrametime() / 10))) + position;
	//acceleration *= 0.99;
	position += velocity;
	if(glm::length(velocity) < 0.001)
		velocity = Vector2(0,0);
	else
		velocity *= 0.8;

	if(velocity != Vector2(0,0))
	{
		direction = glm::normalize(velocity);
		Vector2 up = glm::normalize(Vector2(0, -1));
		rotation = -(glm::atan(direction.y, direction.x) - glm::atan(up.y, up.x));
		if(rotation != rotation) //check for NaN
			rotation = 0;
	}
	hitbox.x = position.x;
	hitbox.y = position.y;
}
