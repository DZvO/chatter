#ifndef _TEXTINPUT_HPP
#define _TEXTINPUT_HPP

#include <iostream>
#include <string>
using namespace std;

#include <SDL/SDL.h>

class TextInput
{
	public:
		void enable();
		void disable();

		//TODO change to wchar, for better international support
		unsigned char get();
	private:
};

#endif
