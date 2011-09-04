#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
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
	const char * response = "Hey client!";
	unsigned int response_size = strlen(response);
	Packet * buffer = new Packet();
	buffer->allocate();
	Address * sender = new Address();
	ReceiveBuffer * recvb = new ReceiveBuffer();

	bool loop = true;
	while(loop)
	{
		if(socket->receive(buffer, sender) > 0)
		{
			cout << "received something from " << *sender << endl;
			//cout << (unsigned int)buffer->flags << " " << buffer->identifier << " " << buffer->number << " " << buffer->packet_count << endl;
			//cout << (const char*) buffer->payload << endl;
			recvb->addPacket(buffer);
			if(recvb->isComplete())
			{
				string by = recvb->getString();
				string text = recvb->getString();
				cout << by << ": " << text << endl;

				delete recvb;
				recvb = new ReceiveBuffer();

				SendBuffer response;
				response.addString("Navi");
				response.addString(string("hey listen! you wrote: ") + text);
				socket->send(&response, sender);

			}

			//loop = false;
		}
		else
		{
			sleep(0, 35000);
		}
	}
	delete sender;
	delete buffer;
	delete socket;
	return 0;
}
