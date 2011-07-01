#include <iostream>
#include <vector>
#include <list>
#include <bitset>
#include <cstring>
using namespace std;

#include <netinet/in.h>

class Buffer
{
	public:
		Buffer();
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
		void add(char* data, int length);
		char* getPacket(int n);

		list<char*>* getPackets();
		unsigned int getPacketCount();
		unsigned int getByteCount();
		unsigned int getCapacity();

		list<char*>* end();

	private:
		//char *buffer;
		list<char*> packets;
		unsigned int capacity;
		unsigned int byteCount;
		unsigned int packetCount;

		unsigned int putPointer;
		unsigned int getPointer;
		static const unsigned short PACKET_SIZE = 1024;

		char get(unsigned int packetNum, unsigned int byteNum);
};

