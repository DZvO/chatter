//TODO fix some valgrind errors -> "Invalid read of size 8"
#include "address.hpp"
//helper
//get sockaddr, ipV4 or ipV6
static void *getInAddrS(const struct sockaddr_storage *ss)
{
	if(ss->ss_family == AF_INET)
	{
		return &(((struct sockaddr_in*)ss)->sin_addr);
	}
	return &(((struct sockaddr_in6*)ss)->sin6_addr);
}

static void *getInAddr(const struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


Address::Address()
{
	port = 0;
}

Address::Address(string targetAddr, unsigned short port)
{
	int rv = 0;
	struct addrinfo hints, *servinfo;
	memset(&hints, 0, sizeof(hints));
	//hints.ai_family = AF_UNSPEC; //let getaddrinfo decide if we use ipv4 or 6
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	//hints.ai_flags = AI_PASSIVE; //optional? hints will be used to bind()
	
	std::string port_str;
	std::stringstream ss;
	ss << port;
	ss >> port_str;
	
	if((rv = getaddrinfo(targetAddr.c_str(), port_str.c_str(), &hints, &servinfo)) != 0)
	{
		cerr << "ERROR:getaddrinfo | " << gai_strerror(rv) << endl;
		return;
	}

	this->port = port;
	this->addr = *((sockaddr_storage*)servinfo->ai_addr);
	this->addr_len = servinfo->ai_addrlen;
	freeaddrinfo(servinfo);
	this->addr_str = targetAddr;
}

bool Address::operator == (const Address & a)
{
	//TODO
	unsigned int a_size = sizeof(getInAddr((sockaddr*)&a.addr));
	unsigned int my_size = sizeof(getInAddr((sockaddr*)&addr));
	bool same = true;
	if(a_size == my_size && port == a.port)
	{
		for(unsigned int i = 0; i < a_size; i++)
		{
			if(*((unsigned char*)&a.addr) == *((unsigned char*)&addr))
			{
				continue;
			}
			else
			{
				same = false;
				break;
			}
		}
	}
	else
	{
		same = false;
	}
	return same;
}

std::ostream& operator<< (std::ostream& stream, const Address adr)
{
	char addressBuffer[INET6_ADDRSTRLEN];
	if(inet_ntop(adr.addr.ss_family, getInAddr((sockaddr*)&adr.addr), addressBuffer, INET6_ADDRSTRLEN) != NULL)
	{
		/*stream << "{family: ";
		switch(adr.addr.ss_family)
		{
			case AF_INET:
				stream << "IPv4";
				break;
			case AF_INET6:
				stream << "IPv6";
				break;
			default:
				stream << "UNSPEC";
				break;
		}*/
		//stream << ", ";
		//stream << "port: " << adr.port << ", ";
		//stream << "addr: " << (const char*) addressBuffer << '}';
		stream << "[" << (const char*) addressBuffer << "]:" << adr.port;
	}
	else
	{
		perror("ERROR:couldn't print Address | ");
	}
	return stream;
}
