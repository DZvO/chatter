#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "lib/glew/glew.h"
#include <SDL/SDL.h>

#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtx/projection.hpp"
#include "lib/glm/gtc/type_ptr.hpp"

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
using namespace motor;

#include "graphics/cube.hpp"
#include "graphics/camera.hpp"

#include "graphics/scenemanager.hpp"

enum STATE { ADDRESS_ENTRY, ACK_WAIT, NAME_ENTRY, NORMAL} state = ADDRESS_ENTRY;

int main (int argc, char * argv[])
{
	Window::getInstance()->create(1366, 768, "inspector gadget!", true);

	bool enable_textinput = false;
	Input * input = new Input();
	if(ifstream("data/preferences.ini"))
	{
		input->loadKeymapping("data/preferences.ini");
	}
	else
	{
		input->addMapping("forward", (SDLKey)44);
		input->addMapping("backward", (SDLKey)111);
		input->saveKeymapping("data/preferences.ini");
	}


	unsigned int color = 0xffffff;//0xff6000;
	string * name = new string("");
	string * line = nullptr;

	Chatlog * chatlog = new Chatlog();
	chatlog->setSize(800.0, 600.0);
	chatlog->setPosition(-((double)Window::getInstance()->getWidth() / 2), ((double)Window::getInstance()->getHeight() / 2));

	chatlog->setLine("Please enter the address of the server");
	//for(int i = 0; i < 40; i++)
	//chatlog->add("\xff""aaaaaa""Lorem ipsum dolor sit amet, lorem ipsum dolor sit amet");
	string * address = new string("");

	Address * server = nullptr;//new Address(*address, 1337);
	Socket * socket = new Socket(1337);
	Packet * packet_buffer = new Packet();
	packet_buffer->allocate();
	unsigned int identifier = 0;

	ReceiveBufferManager * man = new ReceiveBufferManager();

	Cube * cube = new Cube();
	cube->setSize(glm::vec3(4, 4, 4));
	Cube * cube2 = new Cube();
	cube2->setPosition(glm::vec3(6, 7, -8));
	cube2->setSize(glm::vec3(0.5, 0.5, 0.5));
	SceneManager * sm = new SceneManager();
	sm->addObject(cube);
	sm->addObject(cube2);

	while(input->closeRequested() == false)
	{
		switch(state)
		{
			/* case ADDRESS_ENTRY {{{ */
			case ADDRESS_ENTRY:
				{
					input->enableTextmode();
					while(input->refresh())
					{
						if(input->isPressed(Input::kEnter))
						{
							if(*address == "")
							{
								chatlog->setLine("Please enter a address!");
							}
							else
							{
								chatlog->setLine("");
								state = ACK_WAIT;
								chatlog->setLine("Waiting for server, please be patient...");

								//TODO add check if server/Address was initialized properly
								server = new Address(*address, 1337);
								Packet * connect_packet = new Packet();
								connect_packet->allocate();
								connect_packet->type = Packet::CONNECT_PACKET;
								socket->send(connect_packet, server);
							}
						}
						else
						{
							unsigned char c = input->getChar();
							if(c != 0)
							{
								if(c == '\b')
								{
									*address = address->substr(0, address->size() - 1); //substract last character because backspace was pressed
									chatlog->setLine(string("\xff""424242""> ""\xff""ffffff") + *address);
								}
								else
								{
									(*address) += c;
									chatlog->setLine(string("\xff""424242""> ""\xff""ffffff") + *address);
								}
							}
						}
					}
				}
				break;
				/* }}} */
			case ACK_WAIT: /* {{{ */
				{
					input->disableTextmode();
					if(socket->receive(packet_buffer, nullptr) > 1)
					{
						if(packet_buffer->type == Packet::CONNECT_PACKET)
						{
							identifier = (unsigned int)(packet_buffer->payload[0]) | (unsigned int)(packet_buffer->payload[1] << 8) | (unsigned int)(packet_buffer->payload[2] << 16) | (unsigned int)(packet_buffer->payload[3] << 24);
							delete packet_buffer;
							packet_buffer = new Packet();
							packet_buffer->allocate();
							state = NAME_ENTRY;
							chatlog->setLine("Connection established, now enter your name");
						}
					}
					//TODO add time check, and resend ack-request after a certain amount of time

					//ignore all input, except close requests, which are handled in the uppre while loop
					while(input->refresh())
					{
					}
				}
				break; /* }}} */
			case NAME_ENTRY: /* {{{ */
				{
					input->enableTextmode();
					while(input->refresh())
					{
						if(input->isPressed(Input::kEnter))
						{
							if(*name != "")
							{
								state = NORMAL;
								chatlog->add("\xff""888888""You can now chat freely, simply press enter!");
								chatlog->add("\xff""888888""(btw, you can also change your color with /color [rrggbb -> hex])");
								chatlog->setLine("");
							}
							else
							{
								chatlog->setLine("Please enter your name!");
							}
						}
						else
						{
							unsigned char c = input->getChar();
							if(c != 0)
							{
								if(c == '\b')
								{
									*name = name->substr(0, name->size() - 1); //substract last character because backspace was pressed
									chatlog->setLine(string("\xff""424242""> ""\xff""ffffff") + *name);
								}
								else
								{
									(*name) += c;
									chatlog->setLine(string("\xff""424242""> ""\xff""ffffff") + *name);
								}
							}
						}
					}
				}
				break; /* }}} */
			case NORMAL: /* {{{ */
				{
					// network ------------------
					if(socket->receive(packet_buffer, nullptr) > 1)
					{
						if(packet_buffer->type == Packet::DATA_PACKET)
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
						else
						{
							cerr << "wrong packet received" << endl;
						}
					}
					//	--------------------------

					if(input->isPressedSym(input->getMapping("forward")))
					{
						Camera::getInstance()->setPosition(Camera::getInstance()->getPosition() + glm::vec3(0, 0, 0.5));
					}
					if(input->isPressedSym(input->getMapping("backward")))
					{
						Camera::getInstance()->setPosition(Camera::getInstance()->getPosition() + glm::vec3(0, 0, -0.5));
					}

					if(input->isPressedSym(Input::kArrowRight))
					{
						Camera::getInstance()->setRotation(Camera::getInstance()->getRotation() + glm::vec3(0, 0.5, 0));
					}
					if(input->isPressedSym(Input::kArrowLeft))
					{
						Camera::getInstance()->setRotation(Camera::getInstance()->getRotation() + glm::vec3(0, -0.5, 0));
					}

					while(input->refresh())
					{
						if(input->isPressed(Input::kEnter))
						{
							if(enable_textinput)
							{
								if((*line) != "")
								{
									if((*line)[0] != '/')
									{
										Message * sendMessage = new Message();
										sendMessage->by = *name;
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
									}
									else
									{
										if(line->find("/color") != string::npos) // {{{
										{
											if(line->length() >= 8+7)
											{
												std::string color_str = line->substr(7);
												try
												{
													std::string color_str_r = color_str.substr(0, 2);
													if(!hex_string_valid(color_str_r))
														throw("r");
													unsigned int color_r = 0;
													std::stringstream ss_r; 
													ss_r.setf(ios::hex);
													ss_r << std::hex << color_str_r;
													ss_r >> std::hex >> color_r;

													std::string color_str_g = color_str.substr(3, 2);
													if(!hex_string_valid(color_str_g))
														throw("g");
													unsigned int color_g = 0;
													std::stringstream ss_g;
													ss_g << std::hex << color_str_g;
													ss_g >> std::hex >> color_g;

													std::string color_str_b = color_str.substr(6, 2);
													if(!hex_string_valid(color_str_b))
														throw("b");
													unsigned int color_b = 0;
													std::stringstream ss_b;
													ss_b.setf(ios::hex);
													ss_b << std::hex << color_str_b;
													ss_b >> std::hex >> color_b;

													color = (color_r << 16) | (color_g << 8) | (color_b << 0);

													std::string color_str;
													std::stringstream ss;
													ss.width(6);
													ss.setf(ios::hex);
													ss.fill('0');
													ss << std::hex << color;
													//cout << "color:" << color << endl;
													ss >> std::hex >> color_str;
													//cout << color_str_r << " " << color_str_g << " " << color_str_b << " => " << color_str << endl;
													//cout << color_r << " " << color_g << " " << color_b << " " << " => " << color << endl; 

													chatlog->add("\xff""888888""Okay! set color to: ""\xff" + color_str + color_str);
													chatlog->setLine("");
												}
												catch(const char * e)
												{
													chatlog->add("\xff""888888""syntax for /color is '/color rr gg bb', where r/g/b is hex");
													cout << "catch'd \"" << e << "\"" << endl;
												}
											}
											else
											{
												chatlog->add("\xff""888888""syntax for /color is '/color rr gg bb', where r/g/b is hex");
											}
										} // }}}
										else if(line->find("/quit") != string::npos) // {{{
										{
											input->requestClose();
										} // }}}
										else if (line->find("/ft") != string::npos)
										{
											std::stringstream ss;
											ss << Window::getInstance()->getFrametime();
											std::string ft;
											ss >> ft;

											chatlog->add(ft);
										}
										else // {{{
										{
											chatlog->add("\xff""888888""Sorry, don't know what you mean.");
										} // }}}
									}
								}
								delete line;
								input->disableTextmode();
								input->disableKeyRepeat();
								enable_textinput = false;
								chatlog->setLine("");
							}
							else
							{
								enable_textinput = true;
								input->enableTextmode();
								input->enableKeyRepeat();
								line = new std::string();
								chatlog->setLine("\xff""424242""> ");
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
				}
				break; /* }}} */
		}

		cube->tick();
		cube2->tick();

		Window::getInstance()->clear();
		//cube->draw();
		//cube2->draw();
		sm->draw();
		chatlog->draw();
		Window::getInstance()->swap();
	}

	if(server != nullptr)
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

	Window::getInstance()->close();
	delete input;
	delete socket;
	return 0;
}

// vim:foldmethod=marker
