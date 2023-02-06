#ifndef SYSTEM_H
#define SYSTEM_H

#include "Entity.h"

namespace ECS
{
	class World;
	class System
	{
		friend class World;
	public:
		System();
		virtual ~System();

		virtual void onInit();
		virtual void onUpdate();
		virtual void onDestroy();

		const Core::uint32& getExecuteOrder() const;
		void setExecuteOrder(const Core::uint32& executeOrder);

	protected:
		Core::uint32 executeOrder;
		World* world;
	};

	// Sorting for execute orders of Systems
	inline bool operator<(const System& lhs, const System& rhs)
	{
		return lhs.getExecuteOrder() < rhs.getExecuteOrder();
	}


}

#endif