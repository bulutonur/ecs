#ifndef TESTMEMORY_H
#define TESTMEMORY_H

#include "utest.h"
#include <iostream>
#include "Core/MemoryManager.h"
using namespace Core;

TEST(MemoryManager, init)
{
	Core::MemoryManager memman;
	uint64 totalSize = 205;
	uint64 growSize = 64;
	memman.init(totalSize,growSize);
	ASSERT_TRUE(memman.getIsInited());
	std::cout << memman << std::endl;

}

TEST(MemoryManager, alloc)
{
	Core::MemoryManager memman;
	uint64 totalSize = 205;
	uint64 growSize = 64;
	memman.init(totalSize, growSize);
	ASSERT_TRUE(memman.getIsInited());
	int* i1 = memman.alloc<int>(123);
	std::cout << (void*)i1 << std::endl;
	int* i2 = memman.alloc<int>(456);
	std::cout << (void*)i2 << std::endl;
	int* i3 = memman.alloc<int>(15);	
	memman.alloc<int>();
	std::cout << (void*)i3 << std::endl;
	int* i4 = memman.alloc<int>(99);
	memman.alloc<double>();
	std::cout << (void*)i4 << std::endl;
	memman.alloc<int>();
	memman.alloc<int>();
	ASSERT_EQ(123, *i1);
	ASSERT_EQ(456, *i2);
	ASSERT_EQ(15, *i3);
	ASSERT_EQ(99, *i4);
	std::cout << memman << std::endl;
}


TEST(MemoryManager, grow)
{
	Core::MemoryManager memman;
	uint64 totalSize = 8;
	uint64 growSize = 64;
	memman.init(totalSize, growSize);
	ASSERT_TRUE(memman.getIsInited());
	int* i1 = memman.alloc<int>(123);
	std::cout << (void*)i1 << std::endl;
	int* i2 = memman.alloc<int>(456);
	std::cout << (void*)i2 << std::endl;
	int* i3 = memman.alloc<int>(15);
	memman.alloc<int>();
	std::cout << (void*)i3 << std::endl;
	int* i4 = memman.alloc<int>(99);
	memman.alloc<double>();
	std::cout << (void*)i4 << std::endl;
	memman.alloc<int>();
	memman.alloc<int>();
	ASSERT_EQ(123, *i1);
	ASSERT_EQ(456, *i2);
	ASSERT_EQ(15, *i3);
	ASSERT_EQ(99, *i4);
	std::cout << memman << std::endl;
}


TEST(MemoryManager, freeUntil)
{
	Core::MemoryManager memman;
	uint64 totalSize = 150;
	uint64 growSize = 64;
	memman.init(totalSize,growSize);

	int* x = (int*)memman.alloc<int>();
	memman.pushMarker("Level 1");
	memman.alloc<int>();

	std::cout << memman << std::endl;
	memman.pushMarker("Level 2");
	memman.alloc<int>();
	*x = 12;
	std::cout << memman << std::endl;
	memman.popMarker();
	std::cout << memman << std::endl;
	//ASSERT_EQ(1,memman.getUsedMainChunkCount());
	ASSERT_EQ(12, *x);
	*x = 14;
	ASSERT_EQ(14, *x);
	std::cout << memman << std::endl;
	memman.alloc<int>();
	memman.alloc<int>();
	//ASSERT_EQ(3, memman.getUsedMainChunkCount());

	std::cout << memman << std::endl;
}



#endif