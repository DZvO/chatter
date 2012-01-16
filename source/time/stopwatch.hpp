#ifndef _STOPWATCH_HPP
#define _STOPWATCH_HPP

#include <iostream>
#include <sys/time.h>

class StopWatch
{
	public:
		StopWatch();

		void start ();
		void stop ();
		double get ();
		bool isRunning ();

	private:
		timeval tv1;
		timeval tv2;
		bool running;
		bool started;
		unsigned long long delta;
};

#endif
