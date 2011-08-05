#ifndef _CHATLOG_HPP
#define _CHATLOG_HPP
#include <list>
using namespace std;

#include "message.hpp"

class Chatlog
{
	public:
		Chatlog();
		~Chatlog();

		void add (Message * msg);
		list<Message*> * getLog ();

	private:
		list<Message*> log;
};
#endif
