#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP
#include <string>
using namespace std;

class Message
{
	public:
		string by;
		unsigned int by_color; //ARBG
		string text;
		unsigned int text_color; //ARGB

		Message();
};
#endif
