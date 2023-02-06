#ifndef WORLD_H
#define WORLD_H

#include "View.h"
#include "../Core/MemoryManager.h"
#include <iostream>
#include "../Core/Timer.h"

namespace ECS
{
	class World
	{
		friend class System;
		friend class Entity;
	public:
		World(Core::MemoryManager* memoryManager);
		virtual ~World();

		void onInit();
		void onUpdate();
		void onDestroy();

		Entity* addEntity();

		template<typename T, 
			typename std::enable_if<std::is_base_of<System, T>::value>::type* = nullptr,
			typename... Args>
		T* addSystem(Args... args)
		{
			if (allView.systems.has<T>())
			{
				return nullptr;
			}
			T* system = memoryManager->alloc<T>(args...);
			system->world = this;
			allView.systems.add<T>(system);
			orderedSystems.push_back(system);
			return system;
		}


		template<typename T,
			typename std::enable_if<std::is_base_of<System, T>::value>::type* = nullptr>
		void removeSystem()
		{
			if (allView.systems.hasNot<T>())
			{
				return;
			}

			T* system = allView.systems.get<T>();
			std::vector<System*>::iterator it = std::find(orderedSystems.begin(), orderedSystems.end(), system);
			orderedSystems.erase(it);
			allView.systems.remove<T>();
		}

		View all();
	private:

		void reorder();

		std::vector<System*> orderedSystems;
		Entity::Id lastId;

		constexpr static const Entity::Id START_ID = Entity::EMPTY_ID + 1;

		View allView;
		Core::MemoryManager* memoryManager;

		template<typename T,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		void addComponent(T* component)
		{

			if (allView.components.hasNot<T>())
			{
				//@TODO Implement custom vector class
				View::ComponentVector* componentVector = memoryManager->alloc<View::ComponentVector>();
				allView.components.add<T>(componentVector);
			}

			allView.components.get<T,View::ComponentVector>()->push_back(component);

		}

		template<typename T,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		void removeComponent(T* component)
		{

			if (allView.components.hasNot<T>())
			{
				return;
			}
			View::ComponentVector* vector= allView.components.get<T, View::ComponentVector>();
			View::ComponentIterator it = std::find(vector->begin(), vector->end(), component);
			vector->erase(it);
			if (vector->size() == 0)
			{
				allView.components.remove<T>();
			}

		}

	};

	// It has to be added like this because it depends on World
	template<typename T,
		typename std::enable_if<std::is_base_of<Component, T>::value>::type*,
		typename... Args>
	inline T* Entity::addComponent(Args... args)
	{
		if (hasComponent<T>())
		{
			return nullptr;
		}

		T* component = world->memoryManager->alloc<T>(args...);
		component->owner = this;
		components.add<T>(component);
		world->addComponent(component); 
		return component;
	}

	// It has to be added like this because it depends on World
	template<typename T,
		typename std::enable_if<std::is_base_of<Component, T>::value>::type*>
	void Entity::removeComponent()
	{
		if (hasNotComponent<T>())
		{
			return;
		}
		T* component = getComponent<T>();
		components.remove<T>();
		world->removeComponent<T>(component);
	}
	
}
#endif