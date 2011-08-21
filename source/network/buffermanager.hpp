#ifndef _BUFFERMANAGER_HPP
#define _BUFFERMANAGER_HPP

#include <iostream>
using namespace std;

#include "network/buffer.hpp"

class BufferManager
{
	public:
	BufferManager();//TODO
	~BufferManager();//TODO

	bool bufferCompleted();//TODO
	Buffer * getCompletedBuffer();//TODO
};

#endif
