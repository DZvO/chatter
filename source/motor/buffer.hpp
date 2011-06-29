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

		void put(const char& c);
		char get();
		void get (char& c);

		void put (const int& i);			//32 bit int
		void get (int& i);						//32 bit int

		void put (const long long& l);//64 bit int
		void get (long long& l);			//64 bit int

		void put (const float& f);		//32 bit
		void get (float& f);					//32 bit

		void put (const double& d);		//64 bit
		void get (double& d);					//64 bit

		//remember - every time you add something there is a chance the buffer gets reallocated and the pointer you got before wont be valid later
		char* getBuffer();
		unsigned int getByteCount();
		unsigned int getCapacity();

		char* end();

	private:
		char *buffer;
		unsigned int capacity;
		unsigned int byteCount;

		unsigned int putPointer;
		unsigned int getPointer;
};

