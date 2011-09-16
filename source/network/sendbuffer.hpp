#ifndef _SENDBUFFER_HPP
#define _SENDBUFFER_HPP

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include <cstdio>
#include <ctime>
using namespace std;

#include "network/packet.hpp"

class SendBuffer //add* functions
{
	public:
	SendBuffer ();
	~SendBuffer ();

	void addChar (unsigned char c);
	void addShort (unsigned short s);
	void addInt (unsigned int i);
	void addLong (unsigned long l);
	void addFloat (float f);
	void addDouble (double d);
	void addString (string s);

	const list<unsigned char*> * getPackets () const;
	unsigned short getPacketCount () const;
	//unsigned int getChecksum ();
	unsigned int getIdentifier () const;

	private:
	list<unsigned char*> packets;
	unsigned short packetPutPointer;
	unsigned short payloadPutPointer;
	unsigned short packetCount;
	unsigned int identifier;
};

#endif
