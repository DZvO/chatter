#include <iostream>
#include <string>
using namespace std;

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "graphics/window.hpp"
#include "input/input.hpp"
#include "network/socket.hpp"
#include "time/cooldown.hpp"

#include "chat/message.hpp"
#include "graphics/chatlog.hpp"

#include <graphics/glm/glm.hpp>
#include <graphics/glm/gtc/matrix_transform.hpp>
#include <graphics/glm/gtx/projection.hpp>
#include <graphics/glm/gtc/type_ptr.hpp>

int main (int argc, char * argv[])
{
	Window * window = new Window();
	window->create(800, 600, "inspector gadget!");

	bool enable_textinput = false;
	Input * input = new Input();
	Cooldown * cd = new Cooldown();
	std::string * line = NULL;

	Chatlog * chatlog = new Chatlog(window);

	//	BufferManager * man = new BufferManager();
	Socket * socket = new Socket(1337);

	while(input->closeRequested() == false)
	{
		// network ------------------
		/*		if(sokket->receive(man))//TODO

					if(man->bufferCompleted())//TODO
					{
					Buffer * fresh = man->getCompletedBuffer();//TODO
					Message * lastMessage = new Message();
					lastMessage->by = fresh->getString();
					lastMessage->text = fresh->getString();
					chatlog->add(lastMessage);
					delete fresh;
					}
		*/
		//	--------------------------
		while(input->refresh())
		{
			if(input->isPressed(Input::kEnter))
			{
				if(enable_textinput)
				{
					if((*line) != "")
					{
						Message * sendMessage = new Message();
						sendMessage->by = "\xff""ff6000" "DerZauberer" "\xff""ffffff";
						sendMessage->text = *line;
						chatlog->add(sendMessage);

						Buffer * sendBuf = new Buffer();
						sendBuf->addString(sendMessage->by);
						sendBuf->addString(sendMessage->text);

						Address target("localhost", 1337);
						socket->send(*sendBuf, target);
						delete sendBuf;
					}
					delete line;
					input->disableTextmode();
					enable_textinput = false;
					string empty = "";
					chatlog->setLine(&empty);
				}
				else
				{
					enable_textinput = true;
					input->enableTextmode();
					line = new std::string();
					chatlog->setLine(line);
				}
				//cout << "textmode is " << (enable_textinput ? "enabled" : "disabled") << endl;
			}
			else
			{
				if(enable_textinput)
				{
					unsigned char c = input->getChar();
					if(c != 0)
					{
						if(c == '\b')
						{
							*line = line->substr(0, line->size() - 1); //substract last character because backspace was pressed
							chatlog->setLine(line);
						}
						else
						{
							(*line) += c;
							chatlog->setLine(line);
						}
					}
				}
			}
		}

		window->clear();
		chatlog->draw(enable_textinput);
		window->swap();
	}

	window->close();
	delete window;
	delete input;
	delete cd;
	delete socket;
	return 0;
}
