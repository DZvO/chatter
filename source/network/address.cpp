//TODO fix some valgrind errors -> "Invalid read of size 8"
#include "address.hpp"

Address::Address()
{
	port = 0;
}

Address::Address(string targetAddr, unsigned short port)
{
	int rv = 0;
	struct addrinfo hints, *servinfo;
	memset(&hints, 0, sizeof(hints));
	//hints.ai_family = AF_UNSPEC; 
	hints.ai_family = AF_UNSPEC; //TODO preffer IPv6 when widespread deployment arrived
	hints.ai_socktype = SOCK_DGRAM;
	//hints.ai_flags = AI_PASSIVE; //optional? hints will be used to bind()
	
	if((rv = getaddrinfo(targetAddr.c_str(), boost::lexical_cast<std::string>(port).c_str(), &hints, &servinfo)) != 0)
	{
		cerr << "ERROR:getaddrinfo | " << gai_strerror(rv) << endl;
		return;
	}

	//freeaddrinfo(servinfo);
	this->port = port;
	this->addr = *((sockaddr_storage*)servinfo->ai_addr);
	this->addr_len = servinfo->ai_addrlen;
	freeaddrinfo(servinfo);
}

std::ostream& operator<< (std::ostream& stream, const Address adr)
{
	char addressBuffer[INET6_ADDRSTRLEN];
	if(inet_ntop(adr.addr.ss_family, Helper::getInAddr((sockaddr*)&adr.addr), addressBuffer, INET6_ADDRSTRLEN) != NULL)
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
