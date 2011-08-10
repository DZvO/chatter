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
#include "buffer.hpp"
#include "address.hpp"

extern const unsigned int AGREED_BUF_SIZE;
extern const unsigned int MAX_BUF_LEN;

extern const unsigned short HEADER_SIZE; 
extern const unsigned short PACKET_SIZE; 
extern const unsigned short PAYLOAD_SIZE; 

class sokket //'heavily abstracted and oo class :)
{
	public:
		sokket(unsigned short port = 1337, bool host = false);

		int send(const char* input, int length, Address receiver);
		int receive(unsigned char*& output, Address& sender);//length should always be PACKET_SIZE

		int send(const string input, Address receiver);
		int receive(string& output, Address& sender);//dumb, because std::string wont save through '0'

		void send(Buffer& buf, Address to);//sends buf
		int receive(Buffer& buf, Address& from);//return buf

		void close();

	private:
		int sockfd;
		Address m_address;
};
#endif
