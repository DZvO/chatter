#ifndef _RECEIVEBUFFERMANAGER_HPP
#define _RECEIVEBUFFERMANAGER_HPP

#include <iostream>
#include <list>
using namespace std;

#include "network/packet.hpp"
#include "network/receivebuffer.hpp"

class ReceiveBufferManager
{
	public:
	ReceiveBufferManager ();
	~ReceiveBufferManager ();

	void add (Packet * pkt);
	bool hasCompleted ();
	ReceiveBuffer * getCompleted ();

	private:
	list<ReceiveBuffer *> * buffers;
	ReceiveBuffer * completed;
};

#endif
