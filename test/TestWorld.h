#ifndef TESTECS_H
#define TESTECS_H

#include "utest.h"

#include "ECS/World.h"

using namespace ECS;

class TestComponent : public Component
{
public:
	int x;
};

class TestComponent2 : public Component
{
public:
	int y;
};

TEST(Entity, id)
{
	Core::MemoryManager memoryManager(fromMB(32), fromMB(4));
	World world(&memoryManager);
	Entity* e = world.addEntity();
	Entity* e2 = world.addEntity();
	ASSERT_EQ(1, e->getId());
	ASSERT_EQ(2, e2->getId());

}

TEST(World, get_entity)
{

	Core::MemoryManager memoryManager(fromMB(32), fromMB(4));
	World world(&memoryManager);
	Entity* e = world.addEntity();
	Entity* e2 = world.addEntity();
	ASSERT_EQ(2, e2->getId());
	Entity* result=world.all().getEntity(2);
	ASSERT_EQ(result, e2);
	
}

TEST(Entity, addComponent)
{
	Core::MemoryManager memoryManager(fromMB(32), fromMB(4));
	World world(&memoryManager);
	Entity* e = world.addEntity();
	TestComponent* c = e->addComponent<TestComponent>();
	c->x = 123;
	ASSERT_TRUE(e->hasComponent<TestComponent>());
	ASSERT_EQ(123,e->getComponent<TestComponent>()->x);
	ASSERT_EQ(nullptr, world.all().getComponents<TestComponent2>());
	ASSERT_EQ(1, world.all().getComponents<TestComponent>()->size());
}

TEST(Entity, speed_addComponent)
{
	Core::MemoryManager memoryManager(fromMB(32), fromMB(4));
	World world(&memoryManager);
	Core::Timer timer;
	timer.start();
	for (size_t i = 0; i < 4096; i++)
	{
		try
		{
		Entity* e = world.addEntity();
		e->addComponent<TestComponent>();
		e->addComponent<TestComponent2>();
		}
		
		catch (std::exception ex)
		{
			std::cout << "Could not allocate "<< i << std::endl;
		}
		
	}

	std::cout << "Create 4K entities with two Components Elapsed time=" << (Core::float64)timer.getTicks() << " ms" << std::endl;

}

TEST(Entity, removeComponent)
{
	Core::MemoryManager memoryManager(fromMB(32), fromMB(4));
	World world(&memoryManager);
	Entity* e = world.addEntity();
	e->addComponent<TestComponent>();
	Entity* e2 = world.addEntity();
	e2->addComponent<TestComponent>();
	e->removeComponent<TestComponent>(); 
	ASSERT_EQ(1, world.all().getComponents<TestComponent>()->size());
	e2->removeComponent< TestComponent>();
	ASSERT_FALSE(e->hasComponent<TestComponent>());
	ASSERT_EQ(nullptr, e->getComponent<TestComponent>());
	ASSERT_EQ(nullptr, world.all().getComponents<TestComponent>());

}

#endif