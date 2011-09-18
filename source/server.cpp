#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <list>
using namespace std;

#include "network/socket.hpp"
#include "network/packet.hpp"
#include "network/receivebuffer.hpp"
#include "network/sendbuffer.hpp"

void sleep(unsigned int sec, unsigned int usec)
{
	struct timeval tv = {sec, usec};
	select(0, 0, 0, 0, &tv);
}

struct Client
{
	Address addr;
	unsigned int ident;
};

int main(int argc, char* argv[])
{
	Socket * socket = new Socket(1337, true);
	list<Client> clients;
	const char * response = "Hey client!";
	unsigned int response_size = strlen(response);
	Packet * packet_buffer = new Packet();
	packet_buffer->allocate();
	Address * sender = new Address();
	ReceiveBuffer * recvb = new ReceiveBuffer();

	bool loop = true;
	while(loop)
	{
		if(socket->receive(packet_buffer, sender) > 0)
		{
			//cout << "received something from " << *sender << endl;
			//cout << (unsigned int)packet_buffer->type << " " << packet_buffer->identifier << " " << packet_buffer->number << " " << packet_buffer->packet_count << endl;
			if(packet_buffer->type == Packet::DATA_PACKET)
			{
				//cout << (const char*) buffer->payload << endl;
				recvb->addPacket(packet_buffer);
				if(recvb->isComplete())
				{
					unsigned int recv_ident = recvb->getInt();
					string by = recvb->getString();
					unsigned int by_color = recvb->getInt();
					string text = recvb->getString();
					unsigned int text_color = recvb->getInt();
					cout << by << ": " << text << endl;

					delete recvb;
					recvb = new ReceiveBuffer();

					SendBuffer response;
					response.addString(by);
					response.addInt(by_color);
					response.addString(text);
					response.addInt(text_color);
					//TODO the sendbuffer somehow gets changed, maybe add cout for all the things and see where something gets changed?

					for(Client client : clients)
					{
						if(client.ident != recv_ident)
						{
							socket->send(&response, &client.addr);
							cout << "sent to " << client.addr << " " << std::hex << client.ident << endl;
						}
					}
				}
			}
			else if(packet_buffer->type == Packet::CONNECT_PACKET)
			{
				cout << "connect'd!" << *sender << endl;
				srand(time(NULL));
				unsigned int ident = rand();

				Packet * ident_packet = new Packet();
				ident_packet->allocate();
				ident_packet->type = Packet::CONNECT_PACKET;
				ident_packet->payload[0] = ident & 0xff;
				ident_packet->payload[1] = (ident & 0xff00) >> 8;
				ident_packet->payload[2] = (ident & 0xff0000) >> 16;
				ident_packet->payload[3] = (ident & 0xff000000) >> 24;
				ident_packet->payload_size = 4;
				cout << "sent ident: " << ident << endl;

				socket->send(ident_packet, sender);

				Client cl;
				cl.addr = *sender;
				cl.ident = ident;
				clients.push_back(cl);
			}
			else if(packet_buffer->type == Packet::DISCONNECT_PACKET)
			{
				unsigned int ident = (unsigned int)(packet_buffer->payload[0]) | (unsigned int)(packet_buffer->payload[1] << 8) | (unsigned int)(packet_buffer->payload[2] << 16) | (unsigned int)(packet_buffer->payload[3] << 24);
				cout << "disconnect, got ident " << ident << endl;

				for(list<Client>::iterator it = clients.begin(); it != clients.end(); it++)
				{
					if(it->ident == ident)
					{
						clients.erase(it);
						break;
					}
				}

				cout << "disconnect'd! " << *sender << endl;
			}

			cout << endl;
			//loop = false;
		}
		else
		{
			sleep(0, 35000);
		}
	}
	delete sender;
	delete packet_buffer;
	delete socket;
	return 0;
}
