#include "buffer.hpp"

Packet::Packet ()
{
	checksum = timestamp = 0;
	payload = NULL;
}

Packet::~Packet ()
{
	delete [] payload;
}

void Packet::allocate()
{
	payload = new char[PAYLOAD_SIZE];
}

Buffer::Buffer ()
{
	Packet* pkt = new Packet();
	pkt->allocate();
	packets.push_back(pkt);
	memset(packets.back()->payload, 0, PAYLOAD_SIZE);
	capacity = PAYLOAD_SIZE;
	packetCount = 1;

	byteCount = 0;
	putPointer = 0;//HEADER_SIZE;
	getPointer = putPointer;
}

Buffer::~Buffer()
{
	for(list<Packet*>::iterator it = packets.begin(); it != packets.end();)
	{
		delete (*it);
		it++;
	}
}

void Buffer::put (const char& c)
{
	if(byteCount == capacity - 1)
	{
		Packet* pkt = new Packet();
		pkt->allocate();
		packets.push_back(pkt);
		memset(packets.back()->payload, 0, PAYLOAD_SIZE);
		capacity += PAYLOAD_SIZE;
		packetCount += 1;
		
		//for(int i = 0; i < HEADER_SIZE; i++)
			//this->put(0);
	}
	packets.back()->payload[byteCount - ((packetCount - 1) * PAYLOAD_SIZE)] = c;
	//cout << "add \"" << (int)c << "\"(\'" << c << "\') at [" << packetCount - 1 << "][" << byteCount - ((packetCount - 1) * PAYLOAD_SIZE) << "]\n";
	byteCount += 1;
	putPointer += 1;
	getPointer = putPointer; //TODO temporary, in the future have functions that increment {put, get}Pointer
}

// packetNum starts at 0, byteNum is relative and goes from 0 to PAYLOAD_SIZE-1
char Buffer::get (unsigned int packetNum, unsigned int byteNum)
{
	list<Packet*>::iterator it = packets.begin();
	for(unsigned int i = 0; i < packetNum; i++)
	{
		it++;
	}
	return (*it)->payload[byteNum];
}

char Buffer::get ()
{
	getPointer--;
	int listNum = getPointer / PAYLOAD_SIZE;
	int buffNum = getPointer % PAYLOAD_SIZE;

	char rv = get(listNum, buffNum);
	//cout << "get \"" << (int)rv << "\" at [" << listNum << "][" << buffNum << "]" << '\n';
	
	return rv;
}

void Buffer::get (char& c)
{
	//getPointer--;
	//c = buffer[getPointer++];
	c = get();
}

void Buffer::put (const int& i)
{
	unsigned int bigE = htonl(i);
	unsigned char c1 = (bigE & 0xff000000) >> 24;//most significant (on big-endian)
	unsigned char c2 = (bigE & 0x00ff0000) >> 16;
	unsigned char c3 = (bigE & 0x0000ff00) >> 8;
	unsigned char c4 = (bigE & 0x000000ff);			//least significant (on big-endian)

	this->put((char)c1);
	this->put((char)c2);
	this->put((char)c3);
	this->put((char)c4);
}

void Buffer::get (int& i)
{
	unsigned char c4 = get();
	unsigned char c3 = get();
	unsigned char c2 = get();
	unsigned char c1 = get();

	i = (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
	i = ntohl(i);
}

void Buffer::put (const long long& l)
{
	//long long mask1 = 0xffffffffffffffff;
	int i1 = int((l & 0x00000000ffffffff));
	int i2 = int((l & 0xffffffff00000000) >> 32);
	this->put(i2);
	this->put(i1);
}

void Buffer::get (long long& l)
{
	int i1 = 0;
	int i2 = 0;
	this->get(i2);
	this->get(i1);

	//long long l1 = 0;
	//long long l2 = 0;
	//this->get((int&)l2);
	//this->get((int&)l1);
	l = ((long long)i2 << 32) | (long long)i1;
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
	float* flp = reinterpret_cast<float*>(&i);
	f = *flp;
	//f = *(reinterpret_cast<float*>(&i));
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

	double* dbp = reinterpret_cast<double*>(&o);
	d = *dbp;	
	//double out = *(reinterpret_cast<double*>(&o));
	//d = out;
}

void Buffer::add(unsigned char* data, int length)
{
	unsigned int got_checksum = 0;
	unsigned short got_timestamp = 0;
	unsigned short got_flags = 0;

	got_checksum = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
	got_timestamp = (data[5] << 8) | (data[4]);
	got_flags = (data[7] << 8) | (data[6]);
	Packet* pkt = new Packet();
	pkt->allocate();

	pkt->checksum = got_checksum;
	pkt->timestamp = got_timestamp;
	pkt->flags = got_flags;
	memcpy(pkt->payload, data + HEADER_SIZE - 1, PAYLOAD_SIZE); 

	packets.push_back(pkt);

	//TODO add put/get pointer incremention TODO//

	//for(int i = 0; i < length; i++)
	//{
		//this->put(data[i]);
	//}
}

Packet* Buffer::getPacket (int n)
{
	return NULL;
}

list<Packet*>* Buffer::getPackets ()
{
	return &packets;
}

unsigned int Buffer::getPacketCount ()
{
	return packetCount;
}

unsigned int Buffer::getByteCount ()
{
	return byteCount;
}

unsigned int Buffer::getCapacity ()
{
	return capacity;
}

unsigned int Buffer::getChecksum ()
{
	std::size_t chk = 0;
	for(std::list<Packet*>::iterator it = packets.begin(); it != packets.end(); it++)
	{
		std::string str((*it)->payload);
		boost::hash_combine(chk, str);
	}
	return chk;
}

list<Packet*>* Buffer::finish ()
{
	unsigned int chekksum = this->getChecksum();
	unsigned short timestamp = 0; 
	for(std::list<Packet*>::iterator it = packets.begin(); it != packets.end(); it++)
	{
		(*it)->checksum = chekksum;
		(*it)->timestamp = timestamp;
	}
	return &packets;
}
