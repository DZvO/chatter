#include "cooldown.hpp"

Cooldown::Cooldown()
{
}

Cooldown::~Cooldown()
{
}

bool Cooldown::cool(float secs, std::string key)
{
	if(timemap[key] <= SDL_GetTicks())
	{
		timemap[key] = SDL_GetTicks() + (unsigned int)(secs * 1000);
		return true;
	}
	else
	{
		return false;
	}
}

bool Cooldown::hot(float secs, std::string key)
{
	return !cool(secs, key);
}

float Cooldown::getTimeLeft (std::string key)
{
	long tl = timemap[key] - SDL_GetTicks();
	return (tl > 0 ? (float)tl / 1000.0f : 0.0f);
}
