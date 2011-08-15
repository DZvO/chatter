#ifndef _COOLDOWN_HPP
#define _COOLDOWN_HPP

/* TODO use own time class
 */

#include <iostream>
#include <string>
#include <map>
using namespace std;
#include <SDL/SDL.h>

class Cooldown
{
	public:
	Cooldown();
	~Cooldown();

	bool cool (float secs, std::string key);
	bool hot (float secs, std::string key);
	float getTimeLeft (std::string key);
	private:
	map<std::string, unsigned int> timemap;
};

#endif
