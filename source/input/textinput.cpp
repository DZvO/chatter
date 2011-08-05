#include "textinput.hpp"

void TextInput::parse(const char* keyState)
{
	for(char c = 'a'; c <= 'z'; c++)
	{
		if(keyState[c])
		{
			if(keyState[Key::kLShift])
			{
				str += (c - 32); //lower to upper case
			}
			else
				str += c;
		}
	}
}

string TextInput::getStr()
{

}
