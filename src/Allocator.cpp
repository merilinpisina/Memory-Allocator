#include "Allocator.h"
#include <cassert>

Allocator::Allocator(const std::size_t totalSize) : totalSize(totalSize), used(0) {}

Allocator::~Allocator(){
    totalSize = 0;
}