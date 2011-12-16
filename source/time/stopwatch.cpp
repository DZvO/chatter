#include "stopwatch.hpp"

StopWatch::StopWatch ()
{
	delta = 0;
}

void StopWatch::start ()
{
	gettimeofday(&tv, nullptr);
	delta = tv.tv_usec;
}

void StopWatch::stop ()
{
	gettimeofday(&tv, nullptr);
	delta = tv.tv_usec - delta;
}

double StopWatch::get ()
{
	return (double)delta / 1000.0;
}
