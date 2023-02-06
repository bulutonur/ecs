#ifndef ENTITY_H
#define ENTITY_H

#include <type_traits>
#include "Component.h"
#include "../Core/TypeMap.h"

namespace ECS
{

	class World;

	class Entity
	{
		friend class World;
	public:
		Entity();
		~Entity();

		typedef typename Core::uint32 Id;

		const Id& getId() const;

		template<typename T,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* =nullptr, 
			typename... Args>
		T* addComponent(Args... args);

		template<typename T,
			typename std::enable_if<std::is_base_of<Component,T>::value>::type* = nullptr>
		T* getComponent()
		{
			if(hasComponent<T>())
			{ 
				return components.get<T>();
			}
			return nullptr;
		}

		template<typename T,
			typename std::enable_if<std::is_base_of<Component,T>::value>::type* = nullptr>
		bool hasComponent()
		{
			return components.has<T>();
		}

		template<typename T,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		bool hasNotComponent()
		{
			return !hasComponent<T>();
		}
		
		template<typename T,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		void removeComponent();

	protected:
		constexpr const static Id EMPTY_ID = 0;
		Id id;

		Core::TypeMap<Component> components;
		World* world;

	};
}

#endif