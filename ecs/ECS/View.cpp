#include "World.h"

namespace ECS
{
	View::View()
	{

	}


	View::View(const View& other)
	{
		entities = other.entities;
		systems = other.systems;
		components = other.components;
	}

	View::~View()
	{

	}


	bool View::hasEntity(const Entity::Id& id)
	{
		return entities.find(id) != entities.end();
	}

	bool View::hasNotEntity(const Entity::Id& id)
	{
		return !hasEntity(id);
	}


	Entity* View::getEntity(const Entity::Id& id)
	{
		if (hasEntity(id))
		{
			return entities[id];
		}
		return nullptr;
	}

	const View& View::operator=(const View& other)
	{
		entities = other.entities;
		systems = other.systems;
		components = other.components;
		return *this;
	}


}