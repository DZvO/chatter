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
					friend std::ostream & operator<<(std::ostream & os, const bullet & blt)
					{
						os << "Bullet(pos(" << blt.position.x << '|' << blt.position.y << "), vel(" << blt.velocity.x << '|' << blt.velocity.y << "), rotation(" << blt.rotation << "), hitbox(" << blt.hitbox << ')';
						return os;
					}
					bullet():position(0,0), velocity(0,0), rotation(0), hitbox(0,0,0,0)
					{
					}
					Rectangle getHitbox ()
					{
						return Rectangle(position.x, position.y, hitbox.width, hitbox.height);
					}
				};
				std::list<bullet> bullets;
		};
	}
}

#endif
