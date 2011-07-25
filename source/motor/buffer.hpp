/* TODO name the variables "byteCount", "putPointer", "getPointer", "capacity" more meaningfull. right now i dont know what they really mean anymore (because of the change to using Packet)
#include <iostream>
#include <vector>
#include <list>
#include <bitset>
#include <cstring>
using namespace std;

#include <boost/functional/hash.hpp>
#include <netinet/in.h>

const unsigned int AGREED_BUF_SIZE = 1024;
const unsigned int MAX_BUF_LEN = AGREED_BUF_SIZE;
const unsigned short HEADER_SIZE = 8;
const unsigned short PACKET_SIZE = 1024;
const unsigned short PAYLOAD_SIZE = PACKET_SIZE - HEADER_SIZE;

class Packet
{
	public:
		unsigned int checksum;
		unsigned short timestamp;
		unsigned short flags;
		char* payload;

		Packet();
		~Packet();

		void allocate();// payload = new char[PAYLOAD_SIZE];
};

class Buffer
{
	public:
		Buffer();
		~Buffer();
		void put (const char& c);
		char get ();
		void get (char& c);

		void put (const int& i);			//32 bit int
		void get (int& i);						//32 bit int

		void put (const long long& l);//64 bit long int
		void get (long long& l);			//64 bit long int

		void put (const float& f);		//32 bit float
		void get (float& f);					//32 bit float

		void put (const double& d);		//64 bit double
		void get (double& d);					//64 bit double

		//make a deep copy of data, you can delete[] it aftewards
		void add(unsigned char* data, int length);
		Packet* getPacket(int n);

		list<Packet*>* getPackets();
		unsigned int getPacketCount();
		unsigned int getByteCount();
		unsigned int getCapacity();
		unsigned int getChecksum();

		list<Packet*>* finish();

	private:
		//char *buffer;
		list<Packet*> packets;
		unsigned int capacity;
		unsigned int byteCount;
		unsigned int packetCount;

		unsigned int putPointer;
		unsigned int getPointer;

		char get(unsigned int packetNum, unsigned int byteNum);
};

