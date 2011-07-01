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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

#include "helper.hpp"
#include "buffer.hpp"
#include "address.hpp"

const unsigned int AGREED_BUF_SIZE = 1024;
const unsigned int MAX_BUF_LEN = AGREED_BUF_SIZE;

class sokket //'heavily abstracted and oo class :)
{
	public:
		sokket(unsigned short port = 1337, bool bind = false);

		int send(const char* input, int length, Address receiver);
		int receive(char* output, int& length, Address& sender);

		int send(const string input, Address receiver);
		int receive(string& output, Address& sender);

		void send(Buffer& buf, Address to);//sends buf
		void receive(Buffer& buf, Address& from);//return buf

		void close();

	private:
		int sockfd;
		Address m_address;
};
