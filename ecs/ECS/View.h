#ifndef VIEW_H
#define VIEW_H

#include <map>
#include <vector>

#include "System.h"

namespace ECS
{
	class World;
	// Results of queries
	// Holds all systems,entites,compenents of a query
	class View
	{
		friend class World;
	public:
		View();
		View(const View& other);
		~View();

		typedef typename std::vector<Component*> ComponentVector;
		typedef typename ComponentVector::iterator ComponentIterator;

		bool hasEntity(const Entity::Id& id);
		bool hasNotEntity(const Entity::Id& id);
		Entity* getEntity(const Entity::Id& id);

		template <typename T,
			typename std::enable_if<std::is_base_of<System, T>::value>::type* = nullptr>
		bool hasSystem()
		{
			return systems.has<T>();
		}

		template <typename T,
			typename std::enable_if<std::is_base_of<System, T>::value>::type* = nullptr>
			bool hasNotSystem()
		{
			return !hasSystem<T>();
		}

		template <typename T,
			typename std::enable_if<std::is_base_of<System, T>::value>::type* = nullptr>
		T* getSystem()
		{
			if (hasSystem<T>())
			{
				return systems.get<T>();
			}
			return nullptr;
		}

		template <typename T,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		ComponentVector* getComponents()
		{
			if (components.has<T>())
			{
				return components.get<T, ComponentVector>();
			}
			return nullptr;
		}

		// user-defined copy assignment (copy-and-swap idiom)
		const View& operator=(const View &other);

	protected:

		typedef typename Core::TypeMap<ComponentVector> ComponentMap;
		typedef typename std::map<Entity::Id, Entity*> EntityMap;
		typedef typename Core::TypeMap<System> SystemMap;

		SystemMap systems;
		EntityMap entities;
		ComponentMap components;

	};

}

#endif // !VIEW_H
