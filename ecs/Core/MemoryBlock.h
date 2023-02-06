#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <limits>
#include "BasicTypes.h"
#include "Assertion.h"
#include "ByteConversion.h"

namespace Core
{

	class MemoryBlock
	{
		friend class MemoryManager;
	public:
		MemoryBlock();

		~MemoryBlock();

		/**
		 * @brief Checks is initialized and is there available size
		 *
		 * @param requiredSize Required size in bytes  for allocating
		 * @return true Returns true if there is enough space
		 * @return false Returns false if there is not enough space
		 */
		bool canAllocate(const Core::uint64& requiredSize);

		/**
		 * @brief Allocates a memory block.
		 * It should be called before allocating in the block.
		 *
		 * @param requestedTotalSizeInBytes size of total allocation as bytes . It may not be equal to real allocation size
		 * @return Core::uint642 real allocated size as bytes . It may not be equal to requestedTotalSizeInBytes
		 */
		Core::uint64 init(const Core::uint64& requestedTotalSizeInBytes);

		/**
		 * @brief Returns it is allocated whether successfully or not
		 *
		 */
		const bool& getIsInited() const;

	private:
		typedef typename char* Data;

		/**
		 * @brief Allocated continous memory with malloc
		 *
		 */
		Data data;

		/**
		 * @brief Total size of the block
		 * 
		 */
		Core::uint64 totalSize;

		/**
		 * @brief Used size of the block
		 * 
		 */
		Core::uint64 usedSize;

		bool isInited;

		/**
		 * @brief Clears all allocations.
		 *
		 */
		void clean();

		/**
		 * @brief A value indicating block size should not be less than this value.
		 * If available block size is less or equal to this value, allocation will be set as failed
		 * Used only on init
		 *
		 * @see init
		 */
		constexpr static const Core::uint64 MIN_ALLOCATE_SIZE = 1;

		/**
		 * @brief A value indicating block size should not be greater than this value.
		 * If requestedTotalSizeInBytes is greater than this value, requestedTotalSizeInBytes will be setted as this value
		 * Used only on init
		 * SIZE_MAX is max amount of OS can allocate. Source: https://stackoverflow.com/a/9094133
		 *
		 * @see init
		 */
		constexpr static const Core::uint64 MAX_ALLOCATE_SIZE = SIZE_MAX;


		const Core::uint64& getTotalSize() const;

		const Core::uint64& getUsedSize() const;

		const Core::uint64& getFreeSize() const;

	};

}


#endif