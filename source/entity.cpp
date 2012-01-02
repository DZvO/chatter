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
