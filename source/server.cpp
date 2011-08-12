#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

#include "network/buffer.hpp"
#include "network/socket.hpp"

void sleep(unsigned int sec, unsigned int usec)
{
	struct timeval tv = {sec, usec};
	select(0, 0, 0, 0, &tv);
}

int main(int argc, char* argv[])
{
	Socket skt(1337, true);
	string response = "Hey client!";
	Buffer buf;

	while(true)
	{
		Address sender;
		int returned = skt.receive(buf, sender);
		if(returned > 0)
		{
			clog << "server: received " << returned << " bytes, from " << sender << '\n';
			cout << '\"';
			{
				string username("");
				string message("");
				buf.get(username);
				buf.get(message);
				cout << username << " : " << message;
			}
			cout << "\"\n";
			return 0;
		}
		else
		{
			sleep(0, 35000);
		}
	}
	return 0;
}
