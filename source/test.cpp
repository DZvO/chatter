#include <iostream>
using namespace std;
#include <boost/thread.hpp>

#include <sys/time.h>

void sleep(unsigned int sec, unsigned int usec)
{
	struct timeval tv = {sec, usec};
	select(0, 0, 0, 0, &tv);
}

string input;
boost::mutex input_mutex;

void threaded()
{
	bool loop = true;
	while(loop)
	{
		string in;
		std::getline(cin, in);
		boost::mutex::scoped_lock lock(input_mutex);
		input = in;
		if(input == "quit")
			loop = false;
	}
}

int main(int argc, char* argv[])
{
	boost::thread input_thread(threaded);

	while(true)
	{
		input_mutex.lock();
		if(input != "")
		{
			cout << "you typed: " << input << endl;
			if(input == "quit")
			{
				cout << "requested to quit" << endl;
				input_mutex.unlock();
				input_thread.join();
				return 0;
			}
			input = "";
			input_mutex.unlock();
		}
		else
		{
			input_mutex.unlock();
			sleep(1, 0);

			timeval tv;
			gettimeofday(&tv, NULL); 
			cout << '\n' << tv.tv_sec << endl;
		}
	}
	input_thread.join();
	return 0;
}
