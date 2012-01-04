#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include <iostream>
#include "lib/glm/glm.hpp"
#include "math/vector.hpp"
#include "math/rectangle.hpp"

enum EntityType
{
	LIFE_POTION, MANA_POTION, DEBUG_POTION
};

class Entity
{
	public:
		EntityType id;
		Vector2 position;
		Rectangle hitbox;
		bool operator==(const EntityType & tp);
		bool operator==(const Entity & ent);
		const Rectangle getHitbox() const;
};

#endif
