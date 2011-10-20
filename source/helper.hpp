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

static const bool hex_string_valid (std::string input)
{
	for(unsigned int length = 0; length < input.length(); length++)
	{
		char c = input[length];
		if((c < 'a' || c > 'f') && (c < '0' || c > '9'))
		{
			cout << "\"" << c << "\" isn't valid" << endl;
			return false;
		}
	}
	return true;
}


#endif
