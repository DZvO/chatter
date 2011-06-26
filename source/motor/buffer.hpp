#include <iostream>
#include <vector>
#include <bitset>
#include <cstring>
using namespace std;

#include <netinet/in.h>

class Buffer
{
	public:
		Buffer();
		void add(char c);

		void put(char c);
		char get();

		void put (const char& c);
		void get (char& c);

		void put (const int& i);			//32 bit int
		void get (int& i);						//32 bit int

		void put (const long long& l);//64 bit int
		void get (long long& l);			//64 bit int

		void put (const float& f);		//32 bit
		void get (float& f);					//32 bit

		void put (const double& d);		//64 bit
		void get (double& d);					//64 bit

		char* getBuffer();
		unsigned int getByteCount();
		unsigned int getCapacity();

		char* end();

	private:
		char *buffer;
		//list<char[1024]> buffer; TODO
		unsigned int capacity;
		unsigned int byteCount;

		unsigned int putPointer;
		unsigned int getPointer;
};

