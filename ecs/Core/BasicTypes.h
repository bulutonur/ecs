#ifndef BASICTYPES_H
#define BASICTYPES_H

// Basic datatypes to prevent differences between platforms and OSes

#include "BuildConfig.h"

#include <type_traits>

#ifndef COMPILER_MSVC
// stdint.h is part of C99 but MSVC doesn't have it.
#include <stdint.h>         // For intptr_t.
#endif

namespace Core
{

	typedef signed char         schar;
	typedef signed char         int8;
	typedef short               int16;
	// TODO: Remove these type guards.  These are to avoid conflicts with
	// obsolete/protypes.h in the Gecko SDK.
#ifndef _INT32
#define _INT32
	typedef int                 int32;
#endif
	// The NSPR system headers define 64-bit as |long| when possible, except on
	// Mac OS X.  In order to not have typedef mismatches, we do the same on LP64.
	//
	// On Mac OS X, |long long| is used for 64-bit types for compatibility with
	// <inttypes.h> format macros even in the LP64 model.
#if defined(__LP64__) && !defined(OS_MACOSX) && !defined(OS_OPENBSD)
	typedef long                int64;
#else
	typedef long long           int64;
#endif
	// NOTE: unsigned types are DANGEROUS in loops and other arithmetical
	// places.  Use the signed types unless your variable represents a bit
	// pattern (eg a hash value) or you really need the extra bit.  Do NOT
	// use 'unsigned' to express "this value should always be positive";
	// use assertions for this.
	typedef unsigned char      uint8;
	typedef unsigned short     uint16;
	// TODO: Remove these type guards.  These are to avoid conflicts with
	// obsolete/protypes.h in the Gecko SDK.
#ifndef _UINT32
#define _UINT32
	typedef unsigned int       uint32;
#endif
	// See the comment above about NSPR and 64-bit.
#if defined(__LP64__) && !defined(OS_MACOSX) && !defined(OS_OPENBSD)
	typedef unsigned long uint64;
#else
	typedef unsigned long long uint64;
#endif

	typedef float  float32;
	typedef double float64;

	// Check floating dataypes sizes
	static_assert(sizeof(float32) == 4, "float size should be 4");
	static_assert(sizeof(float64) == 8, "double size should be 8");
}


#endif

