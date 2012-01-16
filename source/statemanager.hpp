#ifndef _STATEMANAGER_HPP
#define _STATEMANAGER_HPP

#include <iostream>
#include <vector>
#include <list>
#include "state.hpp"
#include "graphics/spritebatch.hpp"

namespace motor
{
	class StateManager
	{
		public:
			static StateManager * getInstance ();
			void changeState (motor::state::State * newState);
			bool isRunning ();
			void stop ();
			void run ();

			SpriteBatch * getSpriteBatch ();
		private: SpriteBatch spritebatch;

		private:
			StateManager();
			motor::state::State * currentState;
			bool running;
			unsigned int ticks;
	};
}

#endif
