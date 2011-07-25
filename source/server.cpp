#include <boost/lexical_cast.hpp>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

#include "motor/socket.hpp"

void sleep(unsigned int sec, unsigned int usec)
{
	struct timeval tv = {sec, usec};
	select(0, 0, 0, 0, &tv);
}

int main(int argc, char* argv[])
{
	sokket skt(1337, true);
	string response = "Hey client!";
	Buffer buf;

	while(true)
	{
		Address sender;
		int returned = skt.receive(buf, sender);
		if(returned > 0)
		{
			clog << "server: received " << returned << " bytes, from\n" << sender << '\n';
			cout << endl;
			cout << buf.get() << endl;
			cout << buf.get() << endl;
			cout << buf.get() << endl;
			cout << buf.get() << endl;
			cout << buf.get() << endl;
		}
		else
		{
			sleep(0, 35000);
		}
	}
	return 0;
}
