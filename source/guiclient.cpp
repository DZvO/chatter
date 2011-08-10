#include <iostream>
#include <string>
using namespace std;

#include "graphics/window.hpp"
#include "input/input.hpp"
#include "network/socket.hpp"

int main (int argc, char * argv[])
{
	Window * window = new Window();
	window->create(800, 600, "inspector gadget!");

	bool enable_textinput = false;
	Input * input = new Input();
	std::string * line = NULL;

	//TODO Chatlog * chatlog = new Chatlog();
	//TODO ChatlogRenderer * chatrenderer = new ChatlogRenderer();
	//
	//TODO BufferManager * man = new BufferManager();
	Sokket * socket = new Sokket(1337);

	while(input->closeRequested() == false)
	{
		// network ------------------
		if(sokket->receive(man))//TODO
		{
			if(man->bufferCompleted())//TODO
			{
				Buffer * fresh = man->getCompletedBuffer();//TODO
				Message * lastMessage = new Message();//TODO
				lastMessage->by = fresh->getString();//TODO
				lastMessage->text = fresh->getString();//TODO
				chatlog->add(lastMessage);//TODO
				delete fresh;
			}
		}
		// --------------------------

		// enable / disable textmode-
		if(input->isPressed(0.2f, Key::kEnter))//TODO
		{
			if(enable_textinput)
			{
				if((*line) != "")
				{
					Message * sendMessage = new Message();//TODO
					sendMessage->by = "MindKontrol";
					sendMessage->text = *line;
					chatlog.add(sendMessage);//TODO add it to our own chatlog

					Buffer * sendBuf = new Buffer();
					sendBuf->add(sendMessage);//TODO

					socket->send(sendBuf);

					delete sendBuf;
				}
				delete line;
				input->disableTextMode();
				enable_textinput = false;
			}
			else
			{
				enable_textinput = true;
				input->enableTextMode();
				line = new std::string();
			}
		}
		// --------------------------

		if(enable_textinput)
		{
			while( (unsigned char c = input->getChar()) != 0)
			{
				(*line) += c;
			}
		}
		else//only handle this if textinput is disabled, sinc the user may want to input a 'q' into his message
		{
			if(input->isPressed(0.2f, Key::kQ))//TODO	
			{
				break;//quit the while loop, and exit the app
			}
		}
	}



	window->close();
	delete window;
	return 0;
}
