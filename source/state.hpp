#ifndef _STATE_HPP
#define _STATE_HPP

#include <iostream>

namespace motor
{
	class StateManager;
	namespace state
	{
		class State
		{
			public:
				State ()
				{
					active = true;
					std::cout << "created state\n";
				}
				virtual ~State ()
				{
					std::cout << "destroyed state\n";
				}
				virtual void init (const motor::StateManager * st) = 0;
				virtual void update (const motor::StateManager * st) = 0;
				virtual void draw (const motor::StateManager * st) = 0;
				bool active;
		};
	}
}
#endif
