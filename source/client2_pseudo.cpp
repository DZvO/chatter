#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

#include "motor/buffer.hpp"
#include "motor/socket.hpp"

void sleep(unsigned int sec, unsigned int usec)
{
	struct timeval tv = {sec, usec};
	select(0, 0, 0, 0, &tv);
}

int main_pseudocode(int argc, char* argv[])
{
	Window * wind = new Window();
	wind->create(800, 600, "chitchat!");

	EventInput * input = new EventInput();
	Chatlog * chatlog = new Chatlog();//chatlog only stores strings, has nothing to do with rendering
	ChatlogRenderer * chatrenderer = new ChatlogRenderer();

	BufferManager * man = new BufferManager();
	while(input->closeReq() == false)
	{
		if(socket.receive(*man))//received something
		{
			Buffer * got = man.getCompleteBuffer();
			if(got != NULL)
			{
				Message * lastMessage = new Message();
				lastMessage->by = got->getString();
				lastMessage->text = got->getString();
				chatlog->add(lastMessage);
				delete got;
			}
		}
		//network stuff
		wind->clear();
		chatrenderer.render(chatlog, "width", "height");
	}

	delete man;
	delete chatrenderer;
	delete chatlog;
	delete input;
	delete wind;

	return 0;
}
//TODO use more OO ->
//TODO CLASSES
//i forgot why but maybe i need a global state thingie ?

class Window {}; //check

//class KbInput {};
/class TextInput { char getChar(); };
/class EventInput { const char* getKeyState(); bool closeReq(); };
class KbInput
{
	bool inputAvailable();

	void enableUnicode();
	char getChar(); 
	void disableUnicode();

	Key getKey(); 
	const bool* getKeys(); 

	bool closeReq(); 
};

class Message { string by, text; }//check
class Chatlog {}//check

class TextRenderer{}
class ChatlogRenderer : protected TextRenderer {}

class BufferManager { hashList<int checksum, Buffer* buffer> list;}
