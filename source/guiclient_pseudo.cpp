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

int new_main_pseudocode(int argc, char* argv[])
{
	Window * wind = new Window();
	wind->create(800, 600, "chitchat!");

	bool enable_textinput = false;
	Input * input = new Input();
	std::string * line = NULL;

	Chatlog * chatlog = new Chatlog();//chatlog only stores strings, has nothing to do with rendering
	ChatlogRenderer * chatrenderer = new ChatlogRenderer();

	BufferManager * man = new BufferManager();
	Sokket * socket = new Sokket(1337); // public Sokket(short port = 4242, bool host = false);
	while(input->closeRequest() == false)
	{
		//network -----------------
		if(socket->receive(man))//received something
		{
			if(man->bufferCompleted()) // check if we completed a buffer, or did we only get part of it / scrap?
			{
				Buffer * fresh = man->getCompletedBuffer();
				Message * lastMessage = new Message();
				lastMessage->by = fresh->getString();
				lastMessage->text = fresh->getString();
				chatlog->add(lastMessage);
				delete fresh;
			}
		}
		//-------------------------

		//enable disable textmode -
		if(input->isPressed(0.2f, Key::kEnter)) //isPressed(float keyDelayInSeconds, Key keyToCheck)
		{
			if(enable_textinput)
			{
				if((*line) != "")
				{
					Message * sendMessage = new Message();
					sendMessage->by = "MindKontrol";
					sendMessage->text = *line;

					Buffer * sendBuf = new Buffer();
					sendBuf->add(sendMessage);

					socket->send(sendBuf);
					delete sendBuf; // only delete the buffer, and not the message since the buffer will call delete on its objects/messages by itself, and calling delete more than once is undefined behaviour.
				}

				delete line;
				input->disableTextMode();
			}
			else
			{
				enable_textinput = true;
				input->enableTextMode();
				line = new std::string();
			}
		}
		//-------------------------

		if(enable_textinput) // handle keycode translated stuff
		{
			while( (unsigned char c = input.getChar()) != 0)
			{
				(*line) += c;
			}
		}
		else // handle keysym stuff
		{
			if(input->isPressed(0.2f, Key::kQ))
			{
				break;
			}
		}

		wind->clear();
		//TODO Bring in *line
		chatrenderer.render(chatlog, "width", "height");
	}

	delete line;
	delete man;
	delete chatrenderer;
	delete chatlog;
	delete input;
	wind->close();
	delete wind;

	return 0;
}
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~

int old_main_pseudocode(int argc, char* argv[])
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
