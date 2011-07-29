#include <boost/lexical_cast.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

#include "motor/address.hpp"
#include "motor/socket.hpp"

void sleep(unsigned int sec, unsigned int usec)
{
	struct timeval tv = {sec, usec};
	select(0, 0, 0, 0, &tv);
}

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		cout << "Please enter your name: ";
		string my_name("");
		std::getline(cin, my_name);
		cout << "\nthanks, now your message: ";
		string message("");
		std::getline(cin, message);
		cout << "\nok, sending... ";

		Buffer sendBuffer;
		sendBuffer.put(my_name);
		sendBuffer.put(message);
		sokket skt = sokket(1337, false);
		Address server = Address(argv[1]);

		cout << "sending buffer with \"" << sendBuffer.getChecksum() << "\" (" << hex << sendBuffer.getChecksum() << dec << " as checksum)" << endl;
		skt.send(sendBuffer, server);
	}
	else
	{
		cout << "Usage \"client {target ip}\"" << endl;
	}
	return 0;
}
