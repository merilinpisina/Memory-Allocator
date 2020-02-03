#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <cassert>

class Allocator {
protected:
    std::size_t totalSize;
    std::size_t used;   
    std::size_t peak;
public:
    Allocator(const std::size_t totalSize);

    virtual ~Allocator();

    virtual void* allocate(const std::size_t size, const std::size_t alignment = 8) = 0;

    virtual void free(void* ptr) = 0;

    virtual void Init() = 0;

    friend class Benchmark;
};

#endif