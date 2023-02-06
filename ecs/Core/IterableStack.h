#ifndef ITERABLE_STACK_H
#define ITERABLE_STACK_H

#include <stack>

namespace Core
{
	/**
	 * @brief A stack which exposes the container. It helps to iterate elements without push/pop
	 * 
	 * @tparam T data type of the stack
	 */
	template<typename T>
	class IterableStack : public std::stack<T> {
	public:
		using std::stack<T>::c; // expose the container

	};
}

#endif