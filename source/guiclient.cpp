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
#include "network/packet.hpp"
#include "network/receivebuffer.hpp"
#include "network/receivebuffermanager.hpp"
#include "network/sendbuffer.hpp"

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

	Socket * socket = new Socket(1337);

	ReceiveBufferManager * man = new ReceiveBufferManager();
	Packet * packet_buffer = new Packet();
	packet_buffer->allocate();

	while(input->closeRequested() == false)
	{
		// network ------------------
		if(socket->receive(packet_buffer, NULL) > 1)
		{
			man->add(packet_buffer);
			if(man->hasCompleted())
			{
				ReceiveBuffer * complete = man->getCompleted();
				Message * recvd = new Message();
				recvd->by = complete->getString();
				recvd->text = complete->getString();
				chatlog->add(recvd);

				delete complete;
				delete packet_buffer;
				packet_buffer = new Packet();
				packet_buffer->allocate();
			}
		}
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
						sendMessage->by = "DerZauberer"; //ff6000
						sendMessage->text = *line;
						chatlog->add(sendMessage);
						cout << "added " << *line << endl;

						Address target("::1", 1337);
						SendBuffer sendBuffer;
						sendBuffer.addString(sendMessage->by);
						sendBuffer.addString(sendMessage->text);
						socket->send(&sendBuffer, &target);
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
				cout << "textmode is " << (enable_textinput ? "enabled" : "disabled") << endl;
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
