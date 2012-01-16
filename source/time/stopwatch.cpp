#include "stopwatch.hpp"

StopWatch::StopWatch ()
{
	delta = 0;
	started = false;
}

void StopWatch::start ()
{
	if(running == false)
	{
		gettimeofday(&tv1, nullptr);
		running = true;
		started = true;
	}
}

void StopWatch::stop ()
{
	gettimeofday(&tv2, nullptr);
	running = false;
}

double StopWatch::get ()
{
	if(running)
		gettimeofday(&tv2, nullptr);
	//backup!
	if(started == false)
		return 0;

	return (double)((tv2.tv_sec - tv1.tv_sec) * 1000000 + tv2.tv_usec - tv1.tv_usec) / 1000.0;
}

bool StopWatch::isRunning ()
{
	return running;
}
