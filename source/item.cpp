#include "item.hpp"
bool Item::operator== (const ItemType & tp)
{
	return (id == tp);
}
bool Item::operator==(const Item & ent)
{
	bool ret = (this->id == ent.id || this->position == ent.position);
	std::cout << "checking: " << ret << '\n';
	return ret;
}

const Rectangle Item::getHitbox () const
{
	return Rectangle(position.x, position.y, hitbox.width, hitbox.height);
}
