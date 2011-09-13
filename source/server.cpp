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

int main(int argc, char* argv[])
{
	Socket * socket = new Socket(1337, true);
	list<Address> clients;
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
			cout << "received something from " << *sender << endl;
			//cout << (unsigned int)packet_buffer->type << " " << packet_buffer->identifier << " " << packet_buffer->number << " " << packet_buffer->packet_count << endl;
			if(packet_buffer->type == Packet::DATA_PACKET)
			{
				//cout << (const char*) buffer->payload << endl;
				recvb->addPacket(packet_buffer);
				if(recvb->isComplete())
				{
					string by = recvb->getString();
					string text = recvb->getString();
					cout << by << ": " << text << endl;

					delete recvb;
					recvb = new ReceiveBuffer();

					SendBuffer response;
					response.addString("Server");
					response.addString("hey listen! " + by + " wrote: " + text);
					for(Address client : clients)
					{
						cout << "looped" << endl;
						socket->send(&response, &client);
						cout << "sent to " << client << endl;
					}
				}
			}
			else if(packet_buffer->type == Packet::CONNECT_PACKET)
			{
				clients.push_back(*sender);
				cout << "connect'd!" << *sender << endl;
				SendBuffer ident;
				srand(time(NULL));
				ident.add(rand());
				socket->send(&ident, &sender);
			}
			else if(packet_buffer->type == Packet::DISCONNECT_PACKET)
			{
				clients.remove(*sender);
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
