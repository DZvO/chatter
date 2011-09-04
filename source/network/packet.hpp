#ifndef _PACKET_HPP
#define _PACKET_HPP

#include <iostream>
#include <cstring>
using namespace std;

const unsigned short MAX_BUF_SIZE = 1024;
const unsigned short HEADER_SIZE = 9;
const unsigned short PACKET_SIZE = 1024;
const unsigned short PAYLOAD_SIZE = PACKET_SIZE - HEADER_SIZE;

class Packet
{
	public:
	unsigned char flags; 				//1
	unsigned int identifier;		//5
	unsigned short number;			//7
	unsigned short packet_count;//9 bytes
	unsigned char * payload;

	Packet ()
	{
		flags = identifier = number = packet_count = 0;
		payload = NULL;
	}
	~Packet ()
	{
		delete [] payload;
	}
	void allocate ()
	{
		payload = new unsigned char [PAYLOAD_SIZE];
		memset(payload, 0, PAYLOAD_SIZE);
	}
};

#endif
