#ifndef _STATE_HPP
#define _STATE_HPP

#include <iostream>

namespace motor
{
	namespace state
	{
		class State
		{
			public:
				State ()
				{
					active = true;
				}
				virtual void init () = 0;
				virtual void update () = 0;
				virtual void draw () = 0;
				bool active;
		};
	}
}
#endif
