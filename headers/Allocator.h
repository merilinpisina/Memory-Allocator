#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "base.h"

class Allocator
{
public:
    Allocator(size_t size, void *start) : start(start), size(size), used_memory(0), num_allocations(0) {}

    virtual ~Allocator()
    {
        assert(num_allocations == 0 && used_memory == 0);
        start = nullptr;
        size = 0;
    }

    virtual void *allocate(size_t size, unsigned alignment = 4) = 0;
    virtual void deallocate(void *p) = 0;

    void *getStart() const
    {
        return start;
    }
    size_t getSize() const
    {
        return size;
    }
    size_t getUsedMemory() const
    {
        return used_memory;
    }
    size_t getNumAllocations() const
    {
        return num_allocations;
    }

protected:
    void *start;
    size_t size;
    size_t used_memory;
    size_t num_allocations;
};

// namespace allocator
namespace allocator
{
template <class T>
T *allocateNew(Allocator &allocator)
{
    return new (allocator.allocate(sizeof(T), __alignof(T))) T;
}

template <class T>
T *allocateNew(Allocator &allocator, const T &t)
{
    return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
}

template <class T>
void deallocateDelete(Allocator &allocator, T &object)
{
    object.~T();
    allocator.deallocate(&object);
}

template <class T>
T *allocateArray(Allocator &allocator, size_t length)
{
    assert(length != 0);
    unsigned headerSize = sizeof(size_t) / sizeof(T);

    if (sizeof(size_t) % sizeof(T) > 0)
        headerSize += 1;

    //Allocate extra space to store array length in the bytes before the array
    T *p = ((T *)allocator.allocate(sizeof(T) * (length + headerSize), __alignof(T))) + headerSize;
    *(((size_t *)p) - 1) = length;

    for (size_t i = 0; i < length; i++)
        new (&p) T;

    return p;
}

template <class T>
void deallocateArray(Allocator &allocator, T *array)
{
    assert(array != nullptr);
    size_t length = *(((size_t *)array) - 1);

    for (size_t i = 0; i < length; i++)
        array.~T();

    //Calculate how much extra memory was allocated to store the length before the array
    unsigned headerSize = sizeof(size_t) / sizeof(T);
    if (sizeof(size_t) % sizeof(T) > 0)
        headerSize += 1;
    allocator.deallocate(array - headerSize);
}
}; 

#endif