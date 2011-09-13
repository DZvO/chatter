#ifndef _ADDRESS_HPP
#define _ADDRESS_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;

#include <unistd.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <boost/lexical_cast.hpp>

#include "helper.hpp"

class Address
{
	public:
		struct sockaddr_storage addr;
		socklen_t addr_len;
		unsigned short port;
		std::string addr_str;
		Address();
		Address(string targetAddr, unsigned short port = 1337);
		bool operator == (const Address & a);
		friend std::ostream& operator<< (std::ostream& stream, const Address adr);
};

#endif
