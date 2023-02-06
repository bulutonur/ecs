#include "World.h"

namespace ECS
{
	Entity::Entity():
		id(EMPTY_ID)
	{

	}

	Entity::~Entity()
	{

	}

	const Entity::Id& Entity::getId() const
	{
		return id;
	}


}