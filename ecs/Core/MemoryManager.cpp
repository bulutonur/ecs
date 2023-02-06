#include "MemoryManager.h"

namespace Core
{
	MemoryManager::MemoryManager() :
		totalSize(0),
		usedSize(0),
		growSize(0),
		currentBlockIndex(0),
		lastStackIndent(0),
#ifdef BUILD_DEBUG
		maxUsedSize(0),
#endif
		isInited(false)
	{

	}


	MemoryManager::MemoryManager(const Core::uint64& requestedTotalSizeInBytes, const Core::uint64& growSizeInBytes)
		:
		totalSize(0),
		usedSize(0),
		growSize(0),
		currentBlockIndex(0),
		lastStackIndent(0),
#ifdef BUILD_DEBUG
		maxUsedSize(0),
#endif
		isInited(false)
	{
		ASSERT(init(requestedTotalSizeInBytes, growSizeInBytes));
	}

	MemoryManager::~MemoryManager()
	{
		clear();
	}

	bool MemoryManager::init(const Core::uint64& requestedTotalSizeInBytes, const Core::uint64& growSizeInBytes)
	{
		if (isInited)
		{
			clear();
		}

		ASSERT(requestedTotalSizeInBytes > 0);
		if (requestedTotalSizeInBytes == 0)
		{
			return false;
		}

		this->growSize = growSizeInBytes;
		ASSERT(growSize > 0);
		if (growSize == 0)
		{
			return false;
		}


		Core::uint64 allocatedSize = allocateBlocks(requestedTotalSizeInBytes);
		if (allocatedSize != requestedTotalSizeInBytes)
		{
			return false;
		}

		this->totalSize = allocatedSize;

		isInited = true;
		return true;
	}

	void MemoryManager::clear()
	{
		if (!isInited)
		{
			return;
		}
		for (size_t i = 0; i < blocks.size(); i++)
		{
			if (blocks[i])
			{
				delete blocks[i];
				blocks[i] = nullptr;
			}
		}
		blocks.clear();
		usedSize = 0;
		totalSize = 0;
		growSize = 0;
#ifdef BUILD_DEBUG
		maxUsedSize = 0;
#endif
		isInited = false;
	}


	void MemoryManager::pushMarker(const std::string& name)
	{
#ifdef BUILD_DEBUG
		std::cout << "PUSH MARKER" << " " << name << "(" << usedSize << ")" << std::endl;
#endif
		markerStack.push(Marker(name, usedSize, lastStackIndent));
		lastStackIndent++;
	}

	void MemoryManager::popMarker()
	{

		ASSERT(!markerStack.empty());

		if (markerStack.empty())
		{
#ifdef BUILD_DEBUG
			std::cout << "POP MARKER" << " " << "EMPTY" << std::endl;
#endif
			return;
		}

		Marker topMarker = markerStack.top();

#ifdef BUILD_DEBUG
		std::cout << "POP MARKER" << " " << topMarker.name << "(" << std::fixed << toMB(usedSize - topMarker.startIndex) << " MB" << ")" << std::endl;
#endif

		usedSize = topMarker.startIndex;
		markerStack.pop();
		lastStackIndent--;
	}

	const bool& MemoryManager::getIsInited() const
	{
		return isInited;
	}


	Core::uint64 MemoryManager::allocateBlocks(const Core::uint64& requestedTotalSizeInBytes)
	{
		Core::uint64 realTotalAllocateSize = 0;

		while (true)
		{
			Core::uint64 requestedBlockAllocateSize = requestedTotalSizeInBytes - realTotalAllocateSize;
			bool isAllocatedEnough = requestedBlockAllocateSize <= MemoryBlock::MIN_ALLOCATE_SIZE;
			if (isAllocatedEnough)
			{
				break;
			}

			MemoryBlock* block = new MemoryBlock();
			const Core::uint64 realBlockAllocateSize = block->init(requestedBlockAllocateSize);
			bool couldNotAllocate = !block->getIsInited();
			if (couldNotAllocate)
			{
				// Delete to prevent memory leak
				delete block;
				break;
			}
#ifdef BUILD_DEBUG
			std::cout << "NEW BLOCK[" << blocks.size() << "]=" << realBlockAllocateSize << " bytes" << std::endl;
#endif
			blocks.push_back(block);
			realTotalAllocateSize += realBlockAllocateSize;
		}
		return realTotalAllocateSize;

	}


	void* MemoryManager::getAllocateAddress(const Core::uint64& requiredSize)
	{
		if (!isInited)
		{
			return nullptr;
		}

		Core::uint64 sizeCounter = 0;
		Core::uint64 tempBlockIndex = currentBlockIndex;
		for (; tempBlockIndex < blocks.size(); tempBlockIndex++)
		{
#ifdef BUILD_DEBUG
			std::cout << "Temp Block Index=" << tempBlockIndex << std::endl;
			std::cout << "Temp Block Size=" << blocks[tempBlockIndex]->usedSize << std::endl;

#endif
			if (usedSize < sizeCounter + blocks[tempBlockIndex]->getTotalSize())
			{
				bool isThereAvailableSizeInCurrentBlock = blocks[tempBlockIndex]->canAllocate(requiredSize);
				if (isThereAvailableSizeInCurrentBlock)
				{

					Core::uint64 sizeIndex = (usedSize - sizeCounter);
#ifdef BUILD_DEBUG
					std::cout << "ALLOCATION" << "(" << tempBlockIndex << "," << sizeIndex << ")" << std::endl;
#endif
					currentBlockIndex = tempBlockIndex;
					blocks[tempBlockIndex]->usedSize += requiredSize;
					return &blocks[tempBlockIndex]->data[sizeIndex];
				}
				else
				{
					// Increase it to keep consistency
					usedSize += blocks[tempBlockIndex]->getFreeSize();
				}
			}
			sizeCounter += blocks[tempBlockIndex]->getTotalSize();

			bool isLastBlock = tempBlockIndex == blocks.size() - 1;
			// If it cant fit new block it will grow continously
			bool canFitForGrow = requiredSize < growSize;
			if (isLastBlock && canFitForGrow)
			{
				// There is not enough size in all blocks. So grow it
				Core::uint64 allocatedSize = allocateBlocks(growSize);
				if (allocatedSize != growSize)
				{
					return nullptr;
				}
			}
		}


		return nullptr;
	}

	const Core::uint64 MemoryManager::getUsedSize() const
	{
		return usedSize;
	}

	std::ostream& operator<<(std::ostream& os, const MemoryManager& rhs)
	{
		os << "Total size:" << std::fixed << toMB(rhs.totalSize) << " MB" << std::endl;
		os << "Current used size:" << std::fixed << toMB(rhs.getUsedSize()) << " MB" << std::endl;
#ifdef BUILD_DEBUG
		os << "Max used size:" << std::fixed << toMB(rhs.maxUsedSize) << " MB" << std::endl;
		os << "Blocks count:" << rhs.blocks.size() << std::endl;
#endif
		Core::uint64 tempSize = 0;
		for (size_t i = 0; i < rhs.markerStack.c.size(); i++)
		{
			MemoryManager::Marker marker = rhs.markerStack.c[i];
			for (size_t i = 0; i < marker.ident; i++)
			{
				std::cout << "+";
			}
			std::cout << marker.name << "(" << std::fixed << toMB(marker.startIndex - tempSize) << " MB" << ")" << std::endl;
			tempSize = marker.startIndex;
		}

		return os;
	}


}