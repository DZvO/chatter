#include "sendbuffer.hpp"

SendBuffer::SendBuffer ()
{
	packetPutPointer = payloadPutPointer = packetCount = 0;
}

SendBuffer::~SendBuffer ()
{
	unsigned int i = 0;
	for(list<unsigned char*>::iterator it = packets.begin(); it != packets.end();)
	{
		cout << "delete " << i++ << endl;
		it++;
		//delete [] *it;
	}
}

void SendBuffer::addChar (unsigned char c)
{
	if(payloadPutPointer >= PAYLOAD_SIZE || packetCount == 0)
	{
		unsigned char * pkt = new unsigned char [PAYLOAD_SIZE];
		memset(pkt, 0, PAYLOAD_SIZE);
		packets.push_back(pkt);
		packetCount++;
		packetPutPointer++;
		payloadPutPointer = 0;
	}
	packets.back()[payloadPutPointer] = c;
	payloadPutPointer++;
}

void SendBuffer::addString (std::string s)
{
	for(unsigned int i = 0; s[i] != '\0'; i++)
	{
		addChar(s[i]);
	}
	addChar('\0');
}

list<unsigned char*> * SendBuffer::getPackets ()
{
	return &packets;
}

unsigned short SendBuffer::getPacketCount ()
{
	return packetCount;
}

unsigned int SendBuffer::getChecksum ()
{
	unsigned int check = 0;
	for(list<unsigned char*>::iterator it = packets.begin(); it != packets.end(); it++)
	{
		for(unsigned int i = 0; i < PAYLOAD_SIZE; i++)
		{
			check ^= (*it)[i];
		}
	}
	return check;
}
