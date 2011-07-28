#include "socket.hpp"
sokket::sokket(unsigned short port, bool bind) : m_address("localhost", port)
{
	/*if(bind)
		{
		int rv = 0;
		addrinfo hints, *servinfo;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;//either ipv4 or ipv6
		hints.ai_socktype = SOCK_DGRAM;//use udp
		hints.ai_flags = AI_PASSIVE;//fill in local ip for me

		if((rv = getaddrinfo(NULL, boost::lexical_cast<std::string>(port).c_str(), &hints, &servinfo)) != 0)
		{
		cerr << "ERROR: getaddrinfo: " << gai_strerror(rv) << endl;
		return;
		}
		m_address.addr = *((sockaddr_storage*)servinfo->ai_addr);
		m_address.addr_len = servinfo->ai_addrlen;
		}*/
	//cout << "new socket (AF_INET=" << AF_INET << ", AF_INET6=" << AF_INET6 << ")\n";
	//cout << "socktype " << m_address.addr.ss_family << endl;
	//m_address = address("localhost", port);
	//TODO allow to select tcp/udp via enum {TCP, UDP}
	if((sockfd = ::socket(m_address.addr.ss_family, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		cerr << "ERROR:socket | " << endl;
		return;
	}
	if(bind == true)
	{
		if(::bind(sockfd, (sockaddr*)&m_address.addr, m_address.addr_len) == -1)
		{
			::close(sockfd);
			perror("ERROR:bind | ");
			return;
		}
	}
	if(fcntl(sockfd, F_SETFL, O_NONBLOCK) != 0)
	{
		cerr << "ERROR:fcntl | " << endl;
		return;
	}
}

int sokket::send(const char* input, int length, Address receiver)
{
	//return helper::send(input.c_str(), 0, input.length(), &receiver.addr, receiver.addr_len, sockfd);
	int sentBytes = 0;

	if((sentBytes = sendto(sockfd, input, length, 0, (struct sockaddr*)&receiver.addr, receiver.addr_len)) == -1)
	{
		perror("ERROR:sendto | ");
		cerr << endl;
	}
	return sentBytes;
}

int sokket::receive(unsigned char*& output, Address& sender)
{
	int recvBytes = 0;
	//unsigned char* buffer = new unsigned char[MAX_BUF_LEN];
	sender.addr_len = sizeof(sender.addr);
	recvBytes = recvfrom(sockfd, output, MAX_BUF_LEN, 0, (struct sockaddr*)&sender.addr, &sender.addr_len);
	sender.addr_len = sizeof(sender.addr);
	return recvBytes;
}

int sokket::send(const string input, Address receiver)
{
	return send(input.c_str(), input.length(), receiver);
}

int sokket::receive(string& output, Address& sender)
{
	//TODO cant wrap my head around why this wont work :[
	/*char* buffer = NULL;
	int length = 0;
	this->receive(buffer, length, sender);
	if(buffer != NULL)
	{
		output = buffer;
	}
	return length;*/
	/*char* recvd = NULL;
	int length = 0;
	int retVal = receive(recvd, length, sender);
	if(retVal > 0)
	{
		output = recvd;
	}
	return retVal;*/

	/*int recvBytes = 0;
	char buffer[AGREED_BUF_SIZE];
	sender.addr_len = sizeof(sender.addr);
	recvBytes = recvfrom(sockfd, buffer, MAX_BUF_LEN - 1, 0, (struct sockaddr*)&sender.addr, &sender.addr_len);
	if(recvBytes > 0)
	{
		buffer[recvBytes] = '\0';
		output = buffer;
	}
	return recvBytes;
	*/
	cout << "ERROR: Not implemented." << endl;
	return 0;
}

void sokket::send(Buffer& buf, Address receiver)
{
	unsigned int checksum = buf.getChecksum();	//dont need to use htonl/ntohl because i use bit shifting ?
	unsigned short timestamp = 0;	//value that gets incremented every packet (1024 bytes)
	unsigned short flags = 0;	//flags, for example what the data should be used for TODO currently un-used -> use for re-send requests?

	unsigned int packet_size = PACKET_SIZE;
	unsigned int payload_size = PAYLOAD_SIZE;
	unsigned int header_size = HEADER_SIZE;

	std::list<Packet*>* list = buf.getPackets();
	std::list<Packet*>::iterator it;
	for(it = list->begin(); it != list->end(); it++)
	{
		char* payload = (*it)->payload;

		char* packet = new char[packet_size];
		//memset(packet, 0, packet_size);

		packet[0] = (checksum & 0x000000ff);
		packet[1] = (checksum & 0x0000ff00) >> 8;
		packet[2] = (checksum & 0x00ff0000) >> 16;
		packet[3] = (checksum & 0xff000000) >> 24;

		packet[4] = (timestamp & 0x00ff);
		packet[5] = (timestamp & 0xff00) >> 8;
		
		packet[6] = (flags & 0x00ff);
		packet[7] = (flags & 0xff00) >> 8;

		memcpy(packet + header_size, payload, payload_size);

		this->send(packet, packet_size, receiver);
		/*for(unsigned int i = 0; i < packet_size; i++)
		{
			if(packet[i] != 0)
				cout << "sent \"" << (int)packet[i] << "\"\n";
		}*/
		delete [] packet;
		timestamp++;
	}
}

int sokket::receive(Buffer& buf, Address& sender)
{
	unsigned char* data = new unsigned char[MAX_BUF_LEN];
	int recvBytes = this->receive(data, sender);
	if(recvBytes > 0)
	{
		/*for(int i = 0; i < recvBytes; i++)
		{
			if(data[i] != 0)
				clog << "recv \"" << (int)data[i] << "\"\n";
		}*/
		buf.add(data);//be naive and pretend we always receive the complete packet?
		//clog << "received " << recvBytes << " bytes(should be: " << PACKET_SIZE << ")" << '\n';
	}
	delete [] data;
	return recvBytes;
}

void sokket::close()
{
	::close(sockfd);
}

