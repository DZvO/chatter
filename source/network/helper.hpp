#ifndef _HELPER_HPP
#define _HELPER_HPP

class Helper
{
	public:
	static void sleep(unsigned int sec, unsigned int usec)
	{
		struct timeval tv = {sec, usec};
		select(0, 0, 0, 0, &tv);
	}

};

#endif
