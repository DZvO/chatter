#ifndef _GAMESTATE_HPP
#define _GAMESTATE_HPP

#include "state.hpp"
#include "statemanager.hpp"
#include "math/vector.hpp"
#include "input/input.hpp"
#include "statemanager.hpp"
#include "keyboardsettingsstate.hpp"
#include "lib/glm/gtx/vector_angle.hpp"
#include <list>
#include "spaceship.hpp"
#include "entity.hpp"

namespace motor
{
	namespace state
	{
		class GameState : public State
		{
			public:
				GameState();
				~GameState();
			private:
				virtual void init (const motor::StateManager * st);
				virtual void update (const motor::StateManager * st);
				virtual void draw (const motor::StateManager * st);

				SpaceShip spaceship;
				std::list<Entity> entitys;

				Image * background;
				Image * tiles;

				void shootLazorBEAM (SpaceShip &);

				struct bullet
				{
					Vector2 position;
					Vector2 velocity;
					float rotation;
					Rectangle hitbox;
				};
				std::list<bullet> bullets;
		};
	}
}

#endif
