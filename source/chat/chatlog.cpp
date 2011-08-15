#include "chatlog.hpp"
Chatlog::Chatlog ()
{
}

Chatlog::~Chatlog ()
{
	for(std::list<Message*>::iterator it = log.begin(); it != log.end(); it++)
	{
		delete (*it);
	}
}

void Chatlog::add (Message * msg)
{
	log.push_back(msg);
}

list<Message*> * getLog ()
{
	return &log;
}
