#ifndef _KBSETTINGSSTATE_HPP
#define _KBSETTINGSSTATE_HPP

#include <iostream>
#include "state.hpp"
#include "statemanager.hpp"
#include "input/input.hpp"
#include "graphics/spritebatch.hpp"

namespace motor
{
	namespace state
	{
		class KeyboardSettingsState : public State
		{
			public:
				KeyboardSettingsState ();
			private:
				virtual void init (const motor::StateManager * st);
				virtual void update (const motor::StateManager * st);
				virtual void draw (const motor::StateManager * st);
		};
	}
}

#endif
