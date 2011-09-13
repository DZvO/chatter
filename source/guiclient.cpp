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

enum STATE
{
	NORMAL, NAME_ENTRY, COLOR_ENTRY
} state = NAME_ENTRY;

const string name_entry_line = "Hey, Listen! Press 'enter' and enter your name!";
const string color_entry_line = "Hey, Listen! Enter your color, or press enter twice to leave it at ffffff (white)";

int main (int argc, char * argv[])
{
	Window * window = new Window();
	window->create(800, 600, "inspector gadget!");

	bool enable_textinput = false;
	Input * input = new Input();
	Cooldown * cd = new Cooldown();
	std::string * line = NULL;

	Chatlog * chatlog = new Chatlog(window);
	chatlog->setLine(name_entry_line);
	string name;
	unsigned int color;

	Address server("192.168.2.100", 1337);
	Socket * socket = new Socket(1337);
	Packet * connect_packet = new Packet();
	connect_packet->allocate();
	connect_packet->type = Packet::CONNECT_PACKET;
	socket->send(connect_packet, &server);

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
				recvd->by_color = 0x424242;
				recvd->text = complete->getString();
				recvd->text_color = 0x858585;
				cout << recvd->by << ": " << recvd->text << endl;
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
						if(state == NORMAL)
						{
							Message * sendMessage = new Message();
							sendMessage->by = name;
							sendMessage->by_color = 0xff6000;
							sendMessage->text = *line;
							chatlog->add(sendMessage);

							SendBuffer sendBuffer;
							sendBuffer.addString(sendMessage->by);
							//sendBuffer.addInt(sendMessage->by_color);
							sendBuffer.addString(sendMessage->text);
							//sendBuffer.addInt(sendMessage->text_color);
							socket->send(&sendBuffer, &server);

							chatlog->setLine("");
						}
						else if(state == NAME_ENTRY)
						{
							name = *line;
							//state = COLOR_ENTRY;
							state = NORMAL;
							chatlog->setLine("You can now chat freely, simply press enter!");
						}
						else if(state == COLOR_ENTRY)
						{
							state = NORMAL;
							//cout << "your color is " << color << endl;
						}
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
					chatlog->setLine("\xff""424242""> ");
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
							chatlog->setLine(string("\xff""424242""> ""\xff""ffffff") + *line);
						}
						else
						{
							(*line) += c;
							chatlog->setLine(string("\xff""424242""> ""\xff""ffffff") + *line);
						}
					}
				}
			}
		}

		window->clear();
		chatlog->draw();
		window->swap();
	}

	Packet * disconnect_packet = new Packet();
	disconnect_packet->allocate();
	disconnect_packet->type = Packet::DISCONNECT_PACKET;
	socket->send(disconnect_packet, &server);

	window->close();
	delete window;
	delete input;
	delete cd;
	delete socket;
	return 0;
}
