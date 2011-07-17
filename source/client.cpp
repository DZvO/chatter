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
	Buffer buf;
	//float inf = 1.47256f;
	buf.put('a');
	buf.put('b');
	buf.put('c');
	buf.put('d');
	buf.put('e');

	sokket skt = sokket(1337, false);
	Address server = Address(argv[1]);
	skt.send(buf, server);

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


/*	sokket skt = sokket(1337, false);
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
*/

	return 0;
}
