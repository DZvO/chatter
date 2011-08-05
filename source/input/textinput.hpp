#ifndef _TEXTINPUT_HPP
#define _TEXTINPUT_HPP

#include <iostream>
#include <string>
using namespace std;

#include "eventinput.hpp"
#include <SDL/SDL.h>

class TextInput
{
	public:
		void parse(const char* keyState);
		string getStr();
	private:
		std::string str;
};

#endif
