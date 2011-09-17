#include "socket.hpp"
Socket::Socket(unsigned short port, bool bind) : m_address("127.0.0.1", 1337)
{
	if(bind)
	{
		int rv = 0;
		struct addrinfo hints, *servinfo;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC; //ipv4 or ipv6, let getaddrinfo decide
		hints.ai_socktype = SOCK_DGRAM; //use udp
		hints.ai_flags = AI_PASSIVE; //used for bind -> use my ip?

		if((rv = getaddrinfo(NULL, "1337", &hints, &servinfo)) != 0)
		{
			cerr << "ERROR : getaddrinfo | " << gai_strerror(rv) << endl;
			return;
		}

		//if((sockfd = ::socket((*((sockaddr_storage*)servinfo->ai_addr)).ss_family, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		if((sockfd = ::socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
		{
			cerr << "ERROR:socket | " << endl;
			return;
		}

		if(::bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
		{
			::close(sockfd);
			perror("ERROR:bind | ");
			return;
		}
		cout << "bound with " << (servinfo->ai_family == AF_INET ? "ipv4" : "ipv6") << endl;

		if(fcntl(sockfd, F_SETFL, O_NONBLOCK) != 0)
		{
			cerr << "ERROR:fcntl | " << endl;
			return;
		}
		freeaddrinfo(servinfo);
	}
	else
	{
		if((sockfd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		{
			cerr << "ERROR:socket | " << endl;
			return;
		}
		if(fcntl(sockfd, F_SETFL, O_NONBLOCK) != 0)
		{
			cerr << "ERROR:fcntl | " << endl;
			return;
		}
	}
}

int Socket::send(const char * input, unsigned int length, const Address * receiver)
{
	//return helper::send(input.c_str(), 0, input.length(), &receiver.addr, receiver.addr_len, sockfd);
	int sentBytes = 0;

	if((sentBytes = sendto(sockfd, input, length, 0, (struct sockaddr*)&receiver->addr, receiver->addr_len)) == -1)
	{
		perror("ERROR:sendto | ");
		cerr << endl;
	}
	return sentBytes;
}

int Socket::receive(unsigned char * output, unsigned int output_cap, Address * sender)
{
	int recvBytes = 0;
	//unsigned char* buffer = new unsigned char[MAX_BUF_LEN];
	if(sender != NULL)
	{
		sender->addr_len = sizeof(sender->addr);
		recvBytes = recvfrom(sockfd, output, output_cap, 0, (struct sockaddr*)&sender->addr, &sender->addr_len);
		sender->addr_len = sizeof(sender->addr);
		sender->port = 0;
	}
	else
	{
		recvBytes = recvfrom(sockfd, output, output_cap, 0, NULL, NULL);
	}
	return recvBytes;
}

void Socket::send(const Packet * pkt, const Address * to)
{
	unsigned char * buffer = new unsigned char [1024];
	memset(buffer, 0, 1024);

	buffer[0] = (pkt->type);
	buffer[1] = (0x000000ff & pkt->identifier) >> 0;
	buffer[2] = (0x0000ff00 & pkt->identifier) >> 8;
	buffer[3] = (0x00ff0000 & pkt->identifier) >> 16;
	buffer[4] = (0xff000000 & pkt->identifier) >> 24;

	buffer[5] = (0x00ff & pkt->number) >> 0;
	buffer[6] = (0xff00 & pkt->number) >> 8;

	buffer[7] = (0x00ff & pkt->packet_count) >> 0;
	buffer[8] = (0xff00 & pkt->packet_count) >> 8;

	memcpy(buffer + HEADER_SIZE, (unsigned char*)pkt->payload, PAYLOAD_SIZE);
	//cout << "sent" << endl;
	//cout << (const char*)buffer + HEADER_SIZE << endl;
	//cout << endl;
	this->send((char*)buffer, 1024, to);
	delete [] buffer;
}

int Socket::receive(Packet * pkt, Address * from)
{
	unsigned char * buffer = new unsigned char [1024];
	memset(buffer, 0, 1024);
	int rv = this->receive(buffer, 1024, from);
	if(rv > 0)
	{
		//cout << "recvd" << endl;
		//cout << (const char*)buffer+HEADER_SIZE << endl;
		//cout << endl;
		pkt->type = buffer[0];
		pkt->identifier = (buffer[1]) | (buffer[2] << 8) | (buffer[3] << 16) | (buffer[4] << 24);
		pkt->number = (buffer[5]) | (buffer[6] << 8);
		pkt->packet_count = (buffer[7]) | (buffer[8] << 8);
		memcpy(pkt->payload, buffer + HEADER_SIZE, PAYLOAD_SIZE);
	}
	delete [] buffer;
	return rv;
}

void Socket::send (const SendBuffer * const buf, const Address * const to)
{
	unsigned int identifier = buf->getIdentifier();
	list<unsigned char*>::const_iterator it = buf->getPackets()->begin();
	Packet * sendpacket = new Packet();
	for(unsigned int i = 0; i < buf->getPacketCount(); i++)
	{
		sendpacket->identifier = identifier;
		sendpacket->number = i;
		sendpacket->packet_count = buf->getPacketCount();
		sendpacket->payload = (*it);

		send(sendpacket, to);
		it++;
	}
	sendpacket->payload = NULL;
	delete sendpacket;
}

void Socket::close()
{
	::close(sockfd);
}

