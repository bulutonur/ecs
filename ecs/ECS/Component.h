#ifndef COMPONENT_H
#define COMPONENT_H

#include "../Core/BasicTypes.h"

namespace ECS
{
	// @TODO Implement serialization
	class Entity;
	class Component
	{
		friend class Entity;
	public:
		Component() :
			owner(nullptr)
		{

		}
		virtual ~Component()
		{

		}

	protected:
		Entity* owner;

	};
}

#endif