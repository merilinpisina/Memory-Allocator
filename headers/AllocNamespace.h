#ifndef ALLOC_NAMESPACE_H
#define ALLOC_NAMESPACE_H

#include "FreeListAllocator.h"

namespace allocator
{

	template <class T> T* allocateNew(Allocator& allocator)
	{
		return new (allocator.allocate(sizeof(T), __alignof(T))) T;
	}

	template <class T> T* allocateNew(Allocator& allocator, const T& t)
	{
		return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
	}

	template <class T> void deallocateDelete(Allocator& allocator, T& object)
	{
		object.~T();
		allocator.free(&object);
	}

	template <class T> T* allocateArray(Allocator& allocator, size_t length)
	{
		assert(length != 0);
		unsigned headerSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0) headerSize += 1;

		//Allocate extra space to store array length in the bytes before the array 
		T* p = ((T*)allocator.allocate(sizeof(T)*(length + headerSize), __alignof(T))) + headerSize;
		*(((size_t*)p) - 1) = length;

		for (size_t i = 0; i < length; i++)
			new (&p) T;

		return p;
	}

	template <class T> void deallocateArray(Allocator& allocator, T* array)
	{
		assert(array != nullptr);
		size_t length = *(((size_t*)array) - 1);

		for (size_t i = 0; i < length; i++) array.~T();

		//Calculate how much extra memory was allocated to store the length before the array 
		unsigned headerSize = sizeof(size_t) / sizeof(T);
		if (sizeof(size_t) % sizeof(T) > 0)
			headerSize += 1;
		allocator.free(array - headerSize);
	}
};

#endif