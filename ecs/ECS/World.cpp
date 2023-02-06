#include "World.h"

#include <algorithm>

namespace ECS
{
	World::World(Core::MemoryManager* memoryManager):
		memoryManager(memoryManager),
		lastId(START_ID)
	{

	}

	World::~World()
	{

	}

	void World::onInit()
	{
		for (size_t i = 0; i < orderedSystems.size(); i++)
		{
			orderedSystems[i]->onInit();
		}
	}

	void World::onUpdate()
	{
		for (size_t i = 0; i < orderedSystems.size(); i++)
		{
			orderedSystems[i]->onUpdate();
		}
	}

	void World::onDestroy()
	{
		for (size_t i = 0; i < orderedSystems.size(); i++)
		{
			orderedSystems[i]->onDestroy();
		}
	}


	void World::reorder()
	{
		std::sort(orderedSystems.begin(), orderedSystems.end());
	}

	Entity* World::addEntity()
	{
		Entity* entity = memoryManager->alloc<Entity>();
		entity->id = lastId;
		entity->world = this;
		allView.entities[entity->getId()] = entity;
		lastId++;
		return entity;
	}

	View World::all()
	{
		return allView;
	}

}