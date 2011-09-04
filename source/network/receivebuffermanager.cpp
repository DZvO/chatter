#include "receivebuffermanager.hpp"

ReceiveBufferManager::ReceiveBufferManager ()
{
	completed = NULL;
	buffers = new list<ReceiveBuffer*>;
}

ReceiveBufferManager::~ReceiveBufferManager ()
{
	//TODO delete buffers
}

void ReceiveBufferManager::add(Packet * pkt)
{
	for(list<ReceiveBuffer*>::iterator it = buffers->begin(); it != buffers->end(); it++)
	{
		if((*it)->getIdentifier() == pkt->identifier)
		{
			(*it)->addPacket(pkt);
			return;
		}
	}

	//Buffer not found, create a new one!
	ReceiveBuffer * toAdd = new ReceiveBuffer();
	toAdd->addPacket(pkt);
	buffers->push_back(toAdd);
}

bool ReceiveBufferManager::hasCompleted ()
{
	for(list<ReceiveBuffer*>::iterator it = buffers->begin(); it != buffers->end(); it++)
	{
		if((*it)->isComplete())
		{
			completed = (*it);
			buffers->erase(it);
			return true;
		}
	}
	completed = NULL;
	return false;
}

ReceiveBuffer * ReceiveBufferManager::getCompleted ()
{
	ReceiveBuffer * rb = completed;
	completed = NULL;
	return rb;
}
