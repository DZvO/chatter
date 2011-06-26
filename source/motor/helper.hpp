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

	//get sockaddr, ipV4 or ipV6
	static void *getInAddrS(const struct sockaddr_storage *ss)
	{
		if(ss->ss_family == AF_INET)
		{
			return &(((struct sockaddr_in*)ss)->sin_addr);
		}
		return &(((struct sockaddr_in6*)ss)->sin6_addr);
	}

	static void *getInAddr(const struct sockaddr *sa)
	{
		if(sa->sa_family == AF_INET)
		{
			return &(((struct sockaddr_in*)sa)->sin_addr);
		}
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}
};

#endif
