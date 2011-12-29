#ifndef _GAMESTATE_HPP
#define _GAMESTATE_HPP

#include "state.hpp"
#include "math/vector.hpp"
#include "input/input.hpp"
#include "statemanager.hpp"
#include "keyboardsettingsstate.hpp"
#include "lib/glm/gtx/vector_angle.hpp"
#include <list>

namespace motor
{
	namespace state
	{
		class GameState : public State
		{
			public:
				GameState();
			private:
				virtual void init ();
				virtual void update ();
				virtual void draw ();

				Vector2 player_position;
				Vector2 player_velocity;
				const Vector2 player_acceleration;
				Vector2 player_direction;
				float player_rotation;
				Image * background;
				Image * tiles;

				float weaponCooldown;
				void shootLazorBEAM (Vector2 normalized_direction);
				struct bullet
				{
					Vector2 position;
					Vector2 velocity;
					float rotation;
				};
				std::list<bullet> bullets;
		};
	}
}

#endif
