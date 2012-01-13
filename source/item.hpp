#ifndef _Item_HPP
#define _Item_HPP

#include <iostream>
#include "lib/glm/glm.hpp"
#include "math/vector.hpp"
#include "math/rectangle.hpp"

enum ItemType
{
	LIFE_POTION, MANA_POTION, DEBUG_POTION
};

class Item
{
	public:
		ItemType id;
		Vector2 position;
		Rectangle hitbox;
		bool operator==(const ItemType & tp);
		bool operator==(const Item & ent);
		const Rectangle getHitbox() const;
};

#endif
