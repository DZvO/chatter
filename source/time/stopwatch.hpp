#ifndef _STOPWATCH_HPP
#define _STOPWATCH_HPP

#include <sys/time.h>

class StopWatch
{
	public:
		StopWatch();

		void start ();
		void stop ();
		double get ();

	private:
		timeval tv;
		unsigned long long delta;
};

#endif
