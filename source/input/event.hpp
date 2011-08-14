#ifndef _EVENT_HPP
#define _EVENT_HPP

#include <iostream>
#include <string>
#include <map>
using namespace std;

#include "input/input.hpp"
#include <SDL/SDL.h>

class Event
{
	public:
	Event();
	~Event();

	void handle();
	void add(std::string key, Input::Key k);
	bool fired(std::string key);
	private:
	Input * input;
	std::map<std::string key, bool value> eventmap;

};
#endif
