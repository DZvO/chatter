#ifndef _BUFFER_HPP
#define _BUFFER_HPP

#include <iostream>
#include <vector>
#include <list>
#include <bitset>
#include <cstring>
#include <string>
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
		char get(unsigned int packetNum, unsigned int byteNum);

		void put (const int& i);			//32 bit int
		void get (int& i);						//32 bit int

		void put (const long long& l);//64 bit long int
		void get (long long& l);			//64 bit long int

		void put (const float& f);		//32 bit float
		void get (float& f);					//32 bit float

		void put (const double& d);		//64 bit double
		void get (double& d);					//64 bit double

		void put (std::string str);		//null terminated string
		void get (std::string& str);

		//-make a deep copy of data, you can delete[] it aftewards
		//-it is expected that data points to a full payload of size PAYLOAD_SIZE
		void add(unsigned char* data);
		Packet* getPacket(unsigned int n);

		list<Packet*>* getPackets();
		unsigned int getPacketCount();
		unsigned int getChecksum();

		unsigned short getPayloadPutPointer();
		unsigned short getPayloadGetPointer();
		unsigned int getPacketPutPointer();
		unsigned int getPacketGetPointer();

		void setPayloadGetPointer(unsigned short i);
		void setPacketGetPointer(unsigned int i);

		list<Packet*>* finish();

		// --- new stuff ---
		inline void addString (std::string str)
		{
			put (str);
		}
		inline std::string getString()
		{
			std::string ret;
			get(ret);
			return ret;
		}
		// ---           ---

	private:
		//char *buffer;
		list<Packet*> packets;
		unsigned int packetCount;

		unsigned short payloadPutPointer;
		unsigned short payloadGetPointer;
		unsigned int packetPutPointer;
		unsigned int packetGetPointer;

};
#endif
