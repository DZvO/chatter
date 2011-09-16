#ifndef _SOCKET_HPP
#define _SOCKET_HPP

#include <unistd.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <list>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

#include "helper.hpp"
#include "network/packet.hpp"
#include "network/address.hpp"
#include "network/sendbuffer.hpp"
#include "network/receivebuffer.hpp"

extern const unsigned short MAX_BUF_SIZE;

extern const unsigned short HEADER_SIZE; 
extern const unsigned short PACKET_SIZE; 
extern const unsigned short PAYLOAD_SIZE; 

class Socket //'heavily abstracted and oo class :)
{
	public:
		Socket(unsigned short port = 1337, bool bind = false);

		int send(const char * input, unsigned int length, const Address * receiver);
		int receive(unsigned char * output, unsigned int output_cap, Address * sender);//length should always be PACKET_SIZE

		void send(const Packet * pkt, const Address * to);
		int receive(Packet * pkt, Address * from);

		void send (const SendBuffer * const buf, const Address * const to);
		//int receive (ReceiveBuffer * buf, Address * from);
		//void send(const Buffer * buf, const Address * to);
		//int receive(Buffer * buf, Address * from);

		void close();

	private:
		int sockfd;
		Address m_address;
};
#endif
