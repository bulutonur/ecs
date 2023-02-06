#include "World.h"

namespace ECS
{
	System::System() :
		world(nullptr),
		executeOrder(0)
	{

	}

	System::~System()
	{

	}

	const Core::uint32& System::getExecuteOrder() const
	{
		return executeOrder;
	}

	void System::setExecuteOrder(const Core::uint32& executeOrder)
	{
		this->executeOrder = executeOrder;
		// Order systems in the world
		world->reorder();
	}

	void System::onInit()
	{

	}

	void System::onUpdate()
	{

	}

	void System::onDestroy()
	{

	}
}