#ifndef _HELPER_HPP
#define _HELPER_HPP
#include <sstream>
template<typename T2, typename T1> inline T2 lexical_cast(const T1 &in)
{
	T2 out;
	std::stringstream ss;
	ss << std::hex << in;
	ss >> std::hex >> out;
	return out;
}
#endif
