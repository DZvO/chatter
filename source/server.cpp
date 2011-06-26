#include <boost/lexical_cast.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

#include "socket.hpp"

void sleep(unsigned int sec, unsigned int usec)
{
	struct timeval tv = {sec, usec};
	select(0, 0, 0, 0, &tv);
}

int main(int argc, char* argv[])
{
	//::buffer buf;
	//buf << 10;

	sokket skt(1337, true);
	string response = "Hey client!";
	string buffer;

	while(true)
	{
		Address sender;
		int returned = skt.receive(buffer, sender);
		if(returned > 0)
		{
			clog << "server: received \"" << buffer << "\" from\n" << sender << '\n';
			//TODO everything now kinda works. but check again to make sure it wont randomly break/crash
			skt.send(response, sender);
		}
		else
		{
			sleep(0, 40000);
		}
	}

	return 0;
}
