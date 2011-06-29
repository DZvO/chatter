#include "buffer.hpp"

Buffer::Buffer()
{
	//buffer = new char[capacity];
	packets.push_back(new char[PACKET_SIZE]);
	capacity = PACKET_SIZE;
	packetCount = 1;

	byteCount = 0;
	putPointer = 0;
	getPointer = 0;
}

void Buffer::put(const char& c)
{
	if(byteCount == capacity)
	{
		packets.push_back(new char[PACKET_SIZE]);
		capacity += PACKET_SIZE;
		packetCount += 1;
		//write header to new packet? TODO
	}
	byteCount += 1;
	packets[packetCount - 1][byteCount - ((packetCount - 1) * PACKET_SIZE)] = c;
	cout << "add \"" << (int)packets[packetCount - 1][byteCount - ((packetCount - 1) * PACKET_SIZE)] << "\" at [" << packetCount - 1 << "][" << byteCount - ((packetCount - 1) * PACKET_SIZE) << "]" << endl;
	//whew long line ^.^
	//cout << "add \"" << (int)buffer[byteCount] << "\" at [" << byteCount << "]" << endl;
}

char Buffer::get()
{
	//if(getPointer > 0)
		//--getPointer;
	//cout << "get \"" << (int)buffer[getPointer] << "\" at [" << getPointer << "]" << endl;
	//return buffer[getPointer++];
	return 0;
}

void Buffer::get(char& c)
{
	//getPointer--;
	//c = buffer[getPointer++];
}

void Buffer::put(const int& i)
{
	unsigned int bigE = htonl(i);
	unsigned char c1 = (bigE & 0xff000000) >> 24;//most significant (on big-endian)
	unsigned char c2 = (bigE & 0x00ff0000) >> 16;
	unsigned char c3 = (bigE & 0x0000ff00) >> 8;
	unsigned char c4 = (bigE & 0x000000ff);			//least significant (on big-endian)

	put(c1);
	put(c2);
	put(c3);
	put(c4);
}

void Buffer::get(int& i)
{
	unsigned char c1 = get();
	unsigned char c2 = get();
	unsigned char c3 = get();
	unsigned char c4 = get();

	i = (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
	i = ntohl(i);
}

void Buffer::put(const long long& l)
{
	//long long mask1 = 0xffffffffffffffff;
	int i1 = int((l & 0x00000000ffffffff));
	int i2 = int((l & 0xffffffff00000000) >> 32);
	this->put(i2);
	this->put(i1);
}

void Buffer::get(long long& l)
{
	//cout << "segfault incomming" << endl;
	long long l1 = 0;
	long long l2 = 0;
	this->get((int&)l2);
	this->get((int&)l1);
	l = (l2 << 32) | l1;
}

void Buffer::put (const float& f)
{
	//cout << "put " << f << endl;
	int i = *(reinterpret_cast<int*>((float*)&f));
	//cout << "ftoi " << i << endl;
	this->put(i);
}

void Buffer::get (float& f)
{
	int i = 0;
	this->get(i);
	//cout << "itof " << i << endl;
	f = *(reinterpret_cast<float*>(&i));
	//cout << f << endl;
}

void Buffer::put (const double& d)
{
	long long i = *(reinterpret_cast<long long*>((double*)&d));
	this->put(i);
}

void Buffer::get (double& d)
{
	long long o = 0;
	this->get(o);
	double out = *(reinterpret_cast<double*>(&o));
	d = out;
}

char* Buffer::getBuffer()
{
	return buffer;
}

unsigned int Buffer::getByteCount()
{
	return byteCount;
}

unsigned int Buffer::getCapacity()
{
	return capacity;
}

char* Buffer::end()
{
	this->put('\0');
	return buffer;
}
