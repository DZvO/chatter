#ifndef _MENUSTATE_HPP
#define _MENUSTATE_HPP

#include <iostream>
#include "state.hpp"
#include "statemanager.hpp"
#include "gamestate.hpp"
#include "keyboardsettingsstate.hpp"

namespace motor
{
	namespace state
	{
		class MenuState : public State
		{
			public:
			MenuState ();
			~MenuState ();
			private:
			virtual void init (const motor::StateManager * st);
			virtual void update (const motor::StateManager * st);
			virtual void draw (const motor::StateManager * st);
			Image * image;
			Image * font;
			int ticks;
		};
	}
}

#endif
