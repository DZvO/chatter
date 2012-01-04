#include "entity.hpp"
bool Entity::operator== (const EntityType & tp)
{
	return (id == tp);
}
bool Entity::operator==(const Entity & ent)
{
	bool ret = (this->id == ent.id || this->position == ent.position);
	std::cout << "checking: " << ret << '\n';
	return ret;
}

const Rectangle Entity::getHitbox () const
{
	return Rectangle(position.x, position.y, hitbox.width, hitbox.height);
}
