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

int sokket::receive(char* output, int& length, Address& sender)
{
	int recvBytes = 0;
	char* buffer = new char[AGREED_BUF_SIZE];
	sender.addr_len = sizeof(sender.addr);
	recvBytes = recvfrom(sockfd, buffer, MAX_BUF_LEN - 1, 0, (struct sockaddr*)&sender.addr, &sender.addr_len);
	if(recvBytes > 0)
	{
		buffer[recvBytes] = '\0';
		output = buffer;
		//clog << "DEBUG: sender family " << sender.addr.ss_family << endl;
	}
	length = recvBytes;
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

	int recvBytes = 0;
	char buffer[AGREED_BUF_SIZE];
	sender.addr_len = sizeof(sender.addr);
	recvBytes = recvfrom(sockfd, buffer, MAX_BUF_LEN - 1, 0, (struct sockaddr*)&sender.addr, &sender.addr_len);
	if(recvBytes > 0)
	{
		buffer[recvBytes] = '\0';
		output = buffer;
	}
	return recvBytes;
}

void sokket::send(const Buffer& buf, Address receiver)
{

}

void sokket::receive(Buffer& buf, Address& sender)
{

}

void sokket::close()
{
	::close(sockfd);
}

