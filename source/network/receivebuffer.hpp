#ifndef _RECEIVEBUFFER_HPP
#define _RECEIVEBUFFER_HPP

#include <iostream>
#include <string>
using namespace std;
#include "network/packet.hpp"

class ReceiveBuffer //only get* functions, and packet sorting and adding
{
	public:
	ReceiveBuffer ();
	~ReceiveBuffer ();
	void addPacket (Packet * pkt);
	bool isComplete ();

	unsigned int getIdentifier ();
	unsigned short getPacketCount ();

	unsigned char getChar ();
	unsigned short getShort ();
	unsigned int getInt ();
	unsigned long getLong ();
	float getFloat ();
	double getDouble ();
	string getString ();

	private:
	unsigned int identifier;
	unsigned short packet_count;
	unsigned short packet_counter;
	bool complete;
	Packet ** packets; // = new Packet [packet_count];

	unsigned short payloadGetPointer;
	unsigned short packetGetPointer;
};

#endif
