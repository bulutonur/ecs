#ifndef BYTECONVERSION_H
#define BYTECONVERSION_H

#include "BasicTypes.h"

namespace Core
{
	constexpr const static uint64 BYTE_UNIT_CONSTANT = 1024;
	/**
	 * @brief Converts kilobytes to bytes
	 *
	 */
	constexpr uint64 fromKB(const uint64& kilobytes)
	{
		return kilobytes * BYTE_UNIT_CONSTANT;
	}

	/**
	 * @brief Converts megabytes to bytes
	 *
	 */
	constexpr uint64 fromMB(const uint64& megabytes)
	{
		return fromKB(megabytes * BYTE_UNIT_CONSTANT);
	}

	/**
	 * @brief Converts gigabytes to bytes
	 *
	 */
	constexpr uint64 fromGB(const uint64& gigabytes)
	{
		return fromMB(gigabytes * BYTE_UNIT_CONSTANT);
	}

	/**
	 * @brief Converts kilobytes to bytes
	 *
	 */
	constexpr double toKB(const uint64& bytes)
	{
		return (double)bytes / (double)BYTE_UNIT_CONSTANT;
	}

	/**
	 * @brief Converts megabytes to bytes
	 *
	 */
	constexpr double toMB(const uint64& bytes)
	{
		return toKB(bytes) / (double)BYTE_UNIT_CONSTANT;
	}

	/**
	 * @brief Converts gigabytes to bytes
	 *
	 */
	constexpr double toGB(const uint64& gigabytes)
	{
		return toMB(gigabytes) / (float)BYTE_UNIT_CONSTANT;
	}



}

#endif
