#include <iostream>
#include <string>
using namespace std;

#include "graphics/window.hpp"
#include "input/input.hpp"
#include "network/socket.hpp"
#include "time/cooldown.hpp"

int main (int argc, char * argv[])
{
	Window * window = new Window();
	window->create(800, 600, "inspector gadget!");

	bool enable_textinput = false;
	Input * input = new Input();
	Cooldown * cd = new Cooldown();
	Event * event = new Event(input);
	event->add("toggle_textmode", Input::kEnter);
	std::string * line = NULL;

	//TODO Chatlog * chatlog = new Chatlog();
	//TODO ChatlogRenderer * chatrenderer = new ChatlogRenderer();
	//
	//TODO BufferManager * man = new BufferManager();
	Socket * socket = new Socket(1337);

	while(input->closeRequested() == false)
	{
		event->handle();
		
		// network ------------------
		//if(sokket->receive(man))//TODO
		//{
		//	if(man->bufferCompleted())//TODO
		//	{
		//		Buffer * fresh = man->getCompletedBuffer();//TODO
		//		Message * lastMessage = new Message();//TODO
		//		lastMessage->by = fresh->getString();//TODO
		//		lastMessage->text = fresh->getString();//TODO
		//		chatlog->add(lastMessage);//TODO
		//		delete fresh;
		//	}
		//}
		// --------------------------

		// enable / disable textmode-
		if(input->isPressed(Input::kEnter))
		{
			if(event->fired("toggle_textmode")) // reset internal bool to false TODO
			{
				if(enable_textinput)
				{
					if((*line) != "")
					{
						//			Message * sendMessage = new Message();//TODO
						//			sendMessage->by = "MindKontrol";
						//			sendMessage->text = *line;
						//			chatlog.add(sendMessage);//TODO add it to our own chatlog

						//			Buffer * sendBuf = new Buffer();
						//			sendBuf->add(sendMessage);//TODO

						//			socket->send(sendBuf);

						//			delete sendBuf;
					}
					delete line;
					input->disableTextmode();
					enable_textinput = false;
				}
				else
				{
					enable_textinput = true;
					input->enableTextmode();
					line = new std::string();
				}
				cout << "textmode is " << (enable_textinput ? "enabled" : "disabled") << endl;
			}
		}
		// --------------------------

		if(enable_textinput)
		{
			unsigned char c = 0;
			while( (c = input->getChar()) != 0)
			{
				(*line) += c;
			}
		}
		else//only handle this if textinput is disabled, since the user may want to input a 'q' into his message
		{
			input->refresh();
			if(input->isPressed(Input::kQ))
			{
				break;//quit the while loop, and exit the app
			}
		}
	}

	window->close();
	delete window;
	return 0;
}
