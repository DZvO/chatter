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
	Buffer buf;
	//int in = 105153;
	//int out = 0;
	//buf.put(in);
	//buf.get(out);
	//cout << out << endl;
	
	//float inf = 1.23456f;
	//float outf = 0.00f;
	//buf.put(inf);
	//buf.get(outf);
	//cout << outf << endl;

	/*long long l = 0x0fffffffffffffff;
	cout << l << endl;
	long long o = 0;
	buf.put(l);
	buf.get(o);
	cout << o << endl;*/

	//TODO check for compat. with 64 and 32 bit
	//TODO also check if little/big endian wont cause any troubles

	/*double di = 1.246;
	double dout = 0.0;
	cout << di << endl;

	buf.put(di);
	buf.get(dout);
	cout << dout << endl;*/


	sokket skt = sokket(1337, false);
	Address server = Address(argv[1]);
	skt.send(argv[2], server);

	cout << "client: sent \"" << (const char*)argv[2] << "\" to\n";
	//helper::printAddress(&server.addr, &server.addr_len);
	cout << server << endl;
	//cout << endl;
	//

	string buffer;
	Address sender;
	while(skt.receive(buffer, sender) < 0) { sleep(0, 50000); }
	cout << "client: received \"" << buffer << "\"\n";
	skt.close();

	return 0;
}
