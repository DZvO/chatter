#ifndef _GAMESTATE_HPP
#define _GAMESTATE_HPP

#include <iostream>
#include "state.hpp"
#include "statemanager.hpp"
#include "math/vector.hpp"
#include "input/input.hpp"
#include "statemanager.hpp"
#include "keyboardsettingsstate.hpp"
#include "lib/glm/gtx/vector_angle.hpp"
#include <list>
#include "player.hpp"
#include "item.hpp"
#include "level.hpp"
#include "time/stopwatch.hpp"

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

				Level level;
				Player player;
				std::list<Item> items;

				Image * background;
				Image * tiles;
				Image * chars;

				unsigned long long ticks;

				void shootLazorBEAM (Player &);

				struct Bullet
				{
					Vector2 position;
					Vector2 velocity;
					Rectangle hitbox;
					friend std::ostream & operator<<(std::ostream & os, const Bullet & blt)
					{
						os << "Bullet(pos(" << blt.position.x << '|' << blt.position.y << "), vel(" << blt.velocity.x << '|' << blt.velocity.y << "), hitbox(" << blt.hitbox << ')';
						return os;
					}

					Bullet():position(0,0), velocity(0,0), hitbox(0,0,0,0)
					{
					}

					Rectangle getHitbox ()
					{
						return Rectangle(position.x, position.y, hitbox.width, hitbox.height);
					}
				};
				std::list<Bullet> bullets;
		};
	}
}

#endif
