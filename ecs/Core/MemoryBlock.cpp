#include "MemoryBlock.h"

namespace Core
{
	MemoryBlock::MemoryBlock() :
		data(nullptr),
		usedSize(0),
		totalSize(0),
		isInited(false)
	{

	}

	MemoryBlock::~MemoryBlock()
	{
		clean();
	}

	bool MemoryBlock::canAllocate(const Core::uint64& requiredSize)
	{
		ASSERT(isInited);

		if (!isInited)
		{
			return false;
		}
#ifdef BUILD_DEBUG
		std::cout << "free=" << std::fixed << toMB(getFreeSize()) << " MB" << std::endl;
#endif
		bool isThereAvailableSize = requiredSize <= getFreeSize();
		return isThereAvailableSize;
	}

	Core::uint64 MemoryBlock::init(const Core::uint64& requestedTotalSizeInBytes)
	{

		ASSERT(!isInited);

		if (isInited)
		{
			return 0;
		}

		ASSERT(requestedTotalSizeInBytes > 0);
		if (requestedTotalSizeInBytes == 0)
		{
			return 0;
		}

		clean();


		Core::uint32 allocateSize = requestedTotalSizeInBytes;
		bool isHigherThanOSLimit = allocateSize > MAX_ALLOCATE_SIZE;
		if (isHigherThanOSLimit)
		{
			allocateSize = MAX_ALLOCATE_SIZE;
		}

		Data availableData = nullptr;
		while (availableData == nullptr)
		{
			availableData = (Data)malloc(allocateSize);
			if (availableData)
			{
				memset(availableData, 0, sizeof(allocateSize));
				break;
			}
			// make it half
			allocateSize = allocateSize >> 1;

			// could not allocate
			if (allocateSize < MIN_ALLOCATE_SIZE)
			{
				return 0;
			}
		}
		data = availableData;
		totalSize = allocateSize;

		isInited = true;
		return allocateSize;
	}

	void MemoryBlock::clean()
	{
		if (!isInited)
		{
			return;
		}

		if (data)
		{
			free(data);
			data = nullptr;
		}
		totalSize = 0;
		usedSize = 0;
		isInited = false;
	}

	const bool& MemoryBlock::getIsInited() const
	{
		return isInited;
	}

	const Core::uint64& MemoryBlock::getTotalSize() const
	{
		return totalSize;
	}

	const Core::uint64& MemoryBlock::getUsedSize() const
	{
		return  usedSize;
	}

	const Core::uint64& MemoryBlock::getFreeSize() const
	{
		return getTotalSize() - getUsedSize();
	}

}