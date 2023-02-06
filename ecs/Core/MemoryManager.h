#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>
#include <vector>
#include <stack>
#include "MemoryBlock.h"
#include "IterableStack.h"

namespace Core
{

	// @TODO Implement thread safeness
	/**
	 * @brief Stack based shared memory pool
	 * Reduces memory fragmentation
	 *
	 * Memory structure:
	 *  ________________________________________
	 * |               |                       |
	 * |     USED      |  ->      FREE         |
	 * |_______________|_______________________|
	 *
	 *
	 * Should call init to set initial size and grow Size.
	 *
	 * IMPORTANT: It does not call the destructor of classes
	 *
	 * It may not one memory block.
	 * Because the OS may not allow large size or RAM may not have free continuous space.
	 * Blocks act like virtually one single block
	 * Blocks in memory are not continuous. For example:
	 *
	 * ........[BLOCK0].........[BLOCK3]......
	 * .......................................
	 * .................[BLOCK2]..............
	 *
	 */
	class MemoryManager
	{

	public:

		MemoryManager(const Core::uint64& requestedTotalSizeInBytes, const Core::uint64& growSizeInBytes);
		MemoryManager();

		~MemoryManager();

		/**
		 * @brief Allocates size of a datatype or class
		 * It does not invoke the destructor of the class while destroying.
		 * But if it is a class, invokes the constructor.
		 * Do not use this method for array allocation.
		 * For arrays, Use a container like std::vector<int*> , then set elements wih alloc<int>
		 *
		 * @tparam T A datatype which will be allocated. For example: int, TestClass
		 * @param args are arguments of the construction.
		 * If T is a class sends args to the constructor else sets it
		 * @return T* pointer of data type
		 */
		template<typename T, typename... Args>
		T* alloc(Args... args)
		{
			const Core::uint64 size = sizeof(T);

#ifdef BUILD_DEBUG
			std::cout << "------";
			std::cout << "Required=" << size << std::endl;
#endif
			void* address = getAllocateAddress(size);
			ASSERT(address != nullptr);
			if (!address)
			{
				// Cant allocate
				throw std::bad_alloc();
				return nullptr;
			}
			// If T is a class sends args to the constructor
			// Else sets it
			new(address) T(args...);

			usedSize += size;

#ifdef BUILD_DEBUG
			if (getUsedSize() > maxUsedSize)
			{
				maxUsedSize = getUsedSize();
			}
#endif
			return (T*)address;
		}

		/**
		 * @brief You should call init to set chunkSize and chunkCount.
		 * Clears if it is already inited.
		 * Real allocated size may not be equal to requestedTotalSizeInBytes. Because there may not be enough free memory available.
		 * So you should check the result if it is ok.
		 *
		 * @param chunkSize size of an element as bytes
		 * @param requestedTotalSizeInBytes size of total allocation as bytes . It may not be equal to real allocation size
		 * @return true if real allocated size is same as requestedTotalSizeInBytes . It may not be equal to requestedTotalSizeInBytes
		 */
		bool init(const Core::uint64& requestedTotalSizeInBytes, const Core::uint64& growSizeInBytes);
		/**
		 * @brief Clears all allocations
		 *
		 * IMPORTANT: It does not call the destructor of the class while destroying
		 */
		void clear();

		/**
		 * @brief A marker to indicate allocation indices. It is helpful to "remove" stack elements
		 * @TODO Implement static string
		 */
		void pushMarker(const std::string& name);

		/**
		 * @brief "Remove"s a marker which is defined by pop.
		 * You have to invoke push before pop
		 *
		 * @see push
		 *
		 * Example usage:
		 * memoryManager.pushMarker("Level1")
		 * // Do allocations
		 * //...
		 * memoryManager.popMarker()
		 *
		 */
		void popMarker();

		/**
		 * @brief Displays the statistics
		 *
		 */
		friend std::ostream& operator<<(std::ostream& os, const MemoryManager& rhs);


		/**
		 * @brief Checks whether memory blocks are allocated
		 * Use this method to check if memory blocks are allocated after init
		 *
		 * @see init
		 *
		 * @return true If it has valid memory blocs
		 * @return false If it has not valid memory blocs
		 */
		const bool& getIsInited() const;

	private:

		// @TODO Implement static string
		/**
		 * @brief Marker for memory regions.
		 * @see pushMarker
		 * @see popMarker
		 * It also may be used for debugging .
		 * 
		 */
		struct Marker
		{
			Marker(const std::string& name, const Core::uint64& startIndex, const Core::uint32& ident = 0) :
				name(name),
				startIndex(startIndex),
				ident(ident)
			{

			}
			/**
			 * @brief Name of the memory region
			 * 
			 */
			std::string name;
			/**
			 * @brief Start index of memory region. Used for calculate the size 
			 * 
			 */
			Core::uint64 startIndex;
			/**
			 * @brief Hierarcy ident.
			 * 
			 */
			Core::uint32 ident;
		};

		Core::uint64 allocateBlocks(const Core::uint64& requestedTotalSizeInBytes);


		/**
		 * @brief  it may not contain single MemoryBlock.
		 * So this function calculates the real block index of chunkIndex and returns it.
		 * Checks is initialized or is there available size
		 *
		 * @param requiredSize Required size for new allocation
		 * @return void* Returns nullptr if there is not enough space.
		 * Else returns memory address of available chunk
		 */
		void* getAllocateAddress(const Core::uint64& requiredSize);

		/**
		 * @brief Holds all chunks
		 */
		std::vector<MemoryBlock*> blocks;

		/**
		 * @brief Total size of all blocks
		 *
		 */
		Core::uint64 totalSize;

		/**
		 * @brief Current used size
		 * If a new allocation is made,it will be increased.
		 * It is helpful to get new allocation index in O(1).
		 * Otherwise, for a new allocation, we would need to iterate blocks.
		 */
		Core::uint64 usedSize;

		/**
		 * @brief is init successful flag to check is it initialized before allocations
		 *
		 */
		bool isInited;

		/**
		 * @brief Holds memory regions
		 * 
		 */
		Core::IterableStack<Marker> markerStack;

		/**
		 * @brief Last memory region ident. Used for hierarchy
		 * 
		 */
		Core::uint32 lastStackIndent;

		/**
		 * @brief Grow size in bytes if there is not available size in all blocks
		 * 
		 */
		Core::uint64 growSize;
		
		/**
		 * @brief Helps to prevent to traverse all memory blocks while getting available index
		 * 
		 */
		Core::uint64 currentBlockIndex;

#ifdef BUILD_DEBUG
		/**
		 * @brief Helps to track max used memory to adjust memory size
		 * Only for debugging purposes.
		 *
		 */

		Core::uint64 maxUsedSize;
#endif

		/**
		 * @brief Gets the used memory size as bytes:
		 *
		 */
		const Core::uint64 getUsedSize() const;
	};

}

#endif