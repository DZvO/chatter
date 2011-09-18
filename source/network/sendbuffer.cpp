#include "sendbuffer.hpp"

SendBuffer::SendBuffer ()
{
	packetPutPointer = payloadPutPointer = packetCount = 0;

	srand(time(NULL));
	identifier = rand();
}

SendBuffer::~SendBuffer ()
{
	unsigned int i = 0;
	for(list<Packet*>::iterator it = packets.begin(); it != packets.end(); it++)
	{
		delete *it;
	}
}

void SendBuffer::addChar (unsigned char c)
{
	if(payloadPutPointer >= PAYLOAD_SIZE || packetCount == 0)
	{
		Packet * pkt = new Packet();
		pkt->allocate(PAYLOAD_SIZE);

		packets.push_back(pkt);
		packetCount++;
		packetPutPointer++;
		payloadPutPointer = 0;
	}
	packets.back()->payload[payloadPutPointer] = c;
	packets.back()->payload_size++;
	payloadPutPointer++;
}

void SendBuffer::addShort (unsigned short s)
{
	addChar((s & 0x00ff));
	addChar((s & 0xff00) >> 8);
}

void SendBuffer::addInt (unsigned int i)
{
	addShort((i & 0x0000ffff));
	addShort((i & 0xffff0000) >> 16);
}

void SendBuffer::addLong (unsigned long l)
{
}

void SendBuffer::addFloat (float f)
{
}

void SendBuffer::addDouble (double d)
{
}

void SendBuffer::addString (std::string s)
{
	for(unsigned int i = 0; s[i] != '\0'; i++)
	{
		addChar(s[i]);
	}
	addChar('\0');
}

const list<Packet*> * SendBuffer::getPackets () const
{
	return &packets;
}

unsigned short SendBuffer::getPacketCount () const
{
	return packetCount;
}

/*unsigned int SendBuffer::getChecksum ()
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
	}*/

unsigned int SendBuffer::getIdentifier () const
{
	return identifier;
}
