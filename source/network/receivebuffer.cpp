#include "receivebuffer.hpp"
ReceiveBuffer::ReceiveBuffer ()
{
	identifier = packet_count = packet_counter = 0;
	packets = NULL;
	complete = false;
	packetGetPointer = payloadGetPointer = 0;
}

ReceiveBuffer::~ReceiveBuffer ()
{
	delete [] packets;
}

void ReceiveBuffer::addPacket (Packet * pkt)
{
	if(packets == NULL && identifier == 0)
	{
		packets = new Packet* [pkt->packet_count];
		packet_count = pkt->packet_count;
		identifier = pkt->identifier;
	}

	packets[pkt->number] = pkt;
	packet_counter++;

	if(packet_counter == packet_count)
	{
		complete = true;
	}
}

bool ReceiveBuffer::isComplete ()
{
	return complete;
}

unsigned int ReceiveBuffer::getIdentifier ()
{
	return identifier;
}

unsigned short ReceiveBuffer::getPacketCount ()
{
	return packet_count;
}

unsigned char ReceiveBuffer::getChar ()
{
	if(complete)
	{
		unsigned char * targetPayload = packets [packetGetPointer]->payload;
		unsigned char rv = targetPayload [payloadGetPointer];
		if(payloadGetPointer >= 1017)
		{
			payloadGetPointer = 0;
			packetGetPointer += 1;
		}
		else
		{
			payloadGetPointer += 1;
		}
		return rv;
	}
	else
	{
		return 0;
	}
}

unsigned short ReceiveBuffer::getShort ()
{
	unsigned short rv = 0;
	rv = ((unsigned short)getChar()) | ((unsigned short)getChar() << 8);
	return rv;
}

unsigned int ReceiveBuffer::getInt ()
{
	unsigned int rv = 0;
	rv = ((unsigned int)getShort()) | ((unsigned int)getShort() << 16);
}

unsigned long ReceiveBuffer::getLong ()
{
	return 0;
}

float ReceiveBuffer::getFloat ()
{
	return 0;
}

double ReceiveBuffer::getDouble ()
{
	return 0;
}

std::string ReceiveBuffer::getString ()
{
	string rv;
	for(char c = (signed char)getChar(); c != '\0'; c = getChar())
	{
		//cout << "got char '" << c << "'\n";
		rv += c;
	}
	//cout << "get \"" << rv << "\"" << endl;
	return rv;
}
