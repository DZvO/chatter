#include "player.hpp"
Player::Player ()
{
	rotation = 0;
	direction = Vector2(1, -1);
	flying = false;
	laser_cooldown = 0.0f;
	jump_cooldown = 0.0f;
	acceleration = Vector2(0, 0);
	gravity = Vector2(0, +1);
	jumping = false;
	jump_duration = 0;
}

void Player::update ()
{
	//velocity = (acceleration * Vector2(Window::getInstance()->getFrametime() / 10)) + velocity;
	//position = (Vector2(0.5) * acceleration * (Vector2(Window::getInstance()->getFrametime() / 10) * Vector2(Window::getInstance()->getFrametime() / 10))) + position;
	//acceleration *= 0.99;
	float delta = position.y;
	position += velocity * Vector2(Window::getInstance()->getFrametime() / 20);
	velocity.y += gravity.y;
	velocity.y += jump.y;
	velocity.x += acceleration.x;

	delta = position.y - delta;

	//cout << "delta: " << delta << "\n";
	//cout << "vely: " << velocity.y << "\n\n";

	if(glm::abs(velocity.x) < 0.001)
		velocity.x = 0.0f;
	else
		velocity.x *= 0.4;

	/*
	direction = glm::normalize(direction);
	if(velocity != Vector2(0,0))
	{
		Vector2 up = glm::normalize(Vector2(0, -1));
		rotation = -(glm::atan(direction.y, direction.x) - glm::atan(up.y, up.x));
		if(rotation != rotation) //check for NaN
			rotation = 0;
	}*/

	if(laser_cooldown > 0.0)
	{
		laser_cooldown -= Window::getInstance()->getFrametime() / 1000.0f;
	}
	if(jump_cooldown > 0.0)
	{
		jump_cooldown -= Window::getInstance()->getFrametime() / 1000.0f;
	}
	if(jumping)
	{
		jump_duration += Window::getInstance()->getFrametime() / 1000.0f;
	}
}
