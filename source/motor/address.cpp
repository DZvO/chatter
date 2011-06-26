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
	hints.ai_family = AF_INET; //TODO preffer IPv6 when widespread deployment arrived
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
}

std::ostream& operator<< (std::ostream& stream, const Address adr)
{
	char addressBuffer[INET6_ADDRSTRLEN];
	if(inet_ntop(AF_INET, Helper::getInAddr((sockaddr*)&adr.addr), addressBuffer, INET6_ADDRSTRLEN) != NULL)
	{
		stream << "family:\t" << adr.addr.ss_family << '\n';
		stream << "port:\t" << adr.port << '\n';
		stream << "addr:\t" << (const char*) addressBuffer << endl;
	}
	else
	{
		perror("ERROR:couldn't print Address | ");
	}
	return stream;
}

