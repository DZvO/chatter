#include <iostream>
#include <string>
using namespace std;

#include <boost/lexical_cast.hpp>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "graphics/window.hpp"
#include "input/input.hpp"
#include "helper.hpp"

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
	CONNECTING, NAME_ENTRY, CONNECTED
} state = CONNECTING;

int main (int argc, char * argv[])
{
	Window * window = new Window();
	window->create(800, 600, "inspector gadget!");

	bool enable_textinput = false;
	Input * input = new Input();
	Cooldown * cd = new Cooldown();
	std::string * line = NULL;

	Chatlog * chatlog = new Chatlog(window);
	string name;
	unsigned int color = 0xffffff;//0xff6000;

	Address * server = new Address("192.168.2.100", 1337);
	Socket * socket = new Socket(1337);
	Packet * packet_buffer = new Packet();
	packet_buffer->allocate();
	unsigned int identifier = 0;

	ReceiveBufferManager * man = new ReceiveBufferManager();

	Packet * connect_packet = new Packet();
	connect_packet->allocate();
	connect_packet->type = Packet::CONNECT_PACKET;
	socket->send(connect_packet, server);
	chatlog->setLine("Waiting for server, please be patient...");

	while(input->closeRequested() == false)
	{
		// network ------------------
		if(socket->receive(packet_buffer, NULL) > 1)
		{
			if(packet_buffer->type == Packet::CONNECT_PACKET)
			{
				identifier = (unsigned int)(packet_buffer->payload[0]) | (unsigned int)(packet_buffer->payload[1] << 8) | (unsigned int)(packet_buffer->payload[2] << 16) | (unsigned int)(packet_buffer->payload[3] << 24);
				cout << "got identifier: " << identifier << endl;
				delete packet_buffer;
				packet_buffer = new Packet();
				packet_buffer->allocate();
				chatlog->setLine("Connected, now please enter your name:");
				state = NAME_ENTRY;
			}
			else if(packet_buffer->type == Packet::DATA_PACKET)
			{
				man->add(packet_buffer);
				if(man->hasCompleted())
				{
					ReceiveBuffer * complete = man->getCompleted();
					Message * recvd = new Message();
					recvd->by = complete->getString();
					recvd->by_color = complete->getInt();
					recvd->text = complete->getString();
					recvd->text_color = complete->getInt();
					cout << recvd->by << ": " << recvd->text << endl;
					chatlog->add(recvd);

					delete complete;
					delete packet_buffer;
					packet_buffer = new Packet();
					packet_buffer->allocate();
				}
			}
		}
		//	--------------------------

		while(input->refresh())
		{
			if(input->isPressed(Input::kR))
			{
				window->resize(600, 400);
			}
			if(input->isPressed(Input::kC))
			{
				window->resize(800, 600);
			}
			if(input->isPressed(Input::kEnter))
			{
				if(enable_textinput)
				{
					if((*line) != "")
					{
						if(state == NAME_ENTRY)
						{
							name = *line;
							//state = COLOR_ENTRY;
							state = CONNECTED;
							chatlog->add("\xff""888888""You can now chat freely, simply press enter!");
							chatlog->add("\xff""888888""(btw, you can also change your color with /color [rrggbb -> hex])");
							chatlog->setLine("");
						}
						else if(state == CONNECTED)
						{
							if((*line)[0] != '/')
							{
								Message * sendMessage = new Message();
								sendMessage->by = name;
								sendMessage->by_color = color;
								sendMessage->text = *line;
								chatlog->add(sendMessage);

								SendBuffer sendBuffer;
								sendBuffer.addInt(identifier);
								sendBuffer.addString(sendMessage->by);
								sendBuffer.addInt(sendMessage->by_color);
								sendBuffer.addString(sendMessage->text);
								sendBuffer.addInt(sendMessage->text_color);
								socket->send(&sendBuffer, server);

								chatlog->setLine("");
							}
							else
							{
								if(line->find("/color") != string::npos)
								{
									try
									{
										color = lexical_cast<unsigned int>(line->substr(7));
										chatlog->add("\xff""888888""Okay! set color to: ""\xff" + lexical_cast<std::string>(color) + line->substr(7));
										chatlog->setLine("");
									}
									catch(exception & e)
									{
										chatlog->setLine("\xff""888888""syntax for /color is '/color rrggbb', where r/g/b is hex");
									}
								}
								else if(line->find("/quit") != string::npos)
								{
									input->requestClose();
								}
								else
								{
									chatlog->add("\xff""888888""Sorry, don't know what you mean.");
								}
							}
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

	if(server != NULL)
	{
		Packet * disconnect_packet = new Packet();
		disconnect_packet->allocate();
		disconnect_packet->type = Packet::DISCONNECT_PACKET;
		disconnect_packet->payload[0] = identifier & 0xff;
		disconnect_packet->payload[1] = (identifier & 0xff00) >> 8;
		disconnect_packet->payload[2] = (identifier & 0xff0000) >> 16;
		disconnect_packet->payload[3] = (identifier & 0xff000000) >> 24;
		disconnect_packet->payload_size = 4;
		socket->send(disconnect_packet, server);
	}

	window->close();
	delete window;
	delete input;
	delete cd;
	delete socket;
	return 0;
}
