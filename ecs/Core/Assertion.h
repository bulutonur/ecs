#ifndef ASSERTION_H
#define ASSERTION_H

// Assertion runs only in BUILD_DEBUG or BUILD_DEVELOPMENT
// It stops the program to see bugs

#include "BuildConfig.h"

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPMENT)

#include <assert.h>
#include <iostream>
#include <string>

inline constexpr void AssertOutput(const bool& exprResult, const std::string& expr, const std::string& file, const int& line)
{
	if (exprResult)
	{

	}
	else
	{
		printf("Assertion Failure:%s\n%s(%d)", expr.c_str(), file.c_str(), line);
		assert(false);
	}

}

// check the expression and fail if it is false
#define ASSERT(expr) AssertOutput(expr,#expr,__FILE__,__LINE__)
#else
#define ASSERT(expr) // evaluates to nothing
#endif


#endif // ! ASSERTIONS_H
