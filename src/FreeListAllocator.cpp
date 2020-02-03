#include "FreeListAllocator.h"
#include "Alignment.h"  
#include <stdlib.h>     
#include <cassert>   
//#include <limits>  
#include <algorithm> 
#include <iostream>

FreeListAllocator::FreeListAllocator(const std::size_t totalSize, const PlacementPolicy pPolicy): Allocator(totalSize) {
    placementPolicy = pPolicy;
}

void FreeListAllocator::Init() {
    if (start != nullptr) {
        free(start);
        start = nullptr;
    }
    start = malloc(totalSize);

    this->Reset();
}

FreeListAllocator::~FreeListAllocator() {
	assert("Memory corruption!" && used == 0);

    free(start);
    start = nullptr;
}

void* FreeListAllocator::allocate(const std::size_t size, const std::size_t alignment) {
    const std::size_t allocationHeaderSize = sizeof(FreeListAllocator::Header);
    const std::size_t freeHeaderSize = sizeof(FreeListAllocator::FreeHeader);
    
	assert("Allocation size must be bigger" && size >= sizeof(Node));
    assert("Alignment must be 8 at least" && alignment >= 8);

    std::size_t padding;
    Node * affectedNode, 
         * previousNode;
    this->Find(size, alignment, padding, previousNode, affectedNode);
    
	assert (affectedNode != nullptr && "Not enough memory");

    const std::size_t alignmentPadding =  padding - allocationHeaderSize;
    const std::size_t requiredSize = size + padding;    

    const std::size_t rest = affectedNode->data.blockSize - requiredSize;

    if (rest > 0) {
        Node * newFreeNode = (Node *)((std::size_t) affectedNode + requiredSize);
        newFreeNode->data.blockSize = rest;
        freeList.insert(affectedNode, newFreeNode);
    }
    freeList.remove(affectedNode);

    // Setup data block
    const std::size_t headerAddress = (std::size_t) affectedNode + alignmentPadding;
    const std::size_t dataAddress = headerAddress + allocationHeaderSize;
    ((FreeListAllocator::Header *) headerAddress)->blockSize = requiredSize;
    ((FreeListAllocator::Header *) headerAddress)->padding = alignmentPadding;

    used += requiredSize;
    peak = std::max(peak, used);

    return (void*) dataAddress;
}

void FreeListAllocator::Find(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    switch (placementPolicy) {
        case FIND_FIRST:
            FindFirst(size, alignment, padding, previousNode, foundNode);
            break;
        case FIND_BEST:
            FindBest(size, alignment, padding, previousNode, foundNode);
            break;
    }
}

void FreeListAllocator::FindFirst(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    Node * it = freeList.head,
         * itPrev = nullptr;
    
    while (it != nullptr) {
        padding = Alignment::CalculatePaddingWithHeader((std::size_t)it, alignment, sizeof (FreeListAllocator::Header));
        const std::size_t requiredSpace = size + padding;
        if (it->data.blockSize >= requiredSpace) {
            break;
        }
        itPrev = it;
        it = it->next;
    }
    previousNode = itPrev;
    foundNode = it;
}

void FreeListAllocator::FindBest(const std::size_t size, const std::size_t alignment, std::size_t& padding, Node *& previousNode, Node *& foundNode) {
    std::size_t smallestDiff = std::numeric_limits<std::size_t>::max();
    Node * bestBlock = nullptr;
    Node * it = freeList.head,
         * itPrev = nullptr;
    while (it != nullptr) {
        padding = Alignment::CalculatePaddingWithHeader((std::size_t)it, alignment, sizeof (FreeListAllocator::Header));
        const std::size_t requiredSpace = size + padding;
        if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff)) {
            bestBlock = it;
        }
        itPrev = it;
        it = it->next;
    }
    previousNode = itPrev;
    foundNode = bestBlock;
}

void FreeListAllocator::free(void* ptr) {
    const std::size_t currentAddress = (std::size_t) ptr;
    const std::size_t headerAddress = currentAddress - sizeof (FreeListAllocator::Header);
    const FreeListAllocator::Header * allocationHeader{ (FreeListAllocator::Header *) headerAddress};

    Node * freeNode = (Node *) (headerAddress);
    freeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->padding;
    freeNode->next = nullptr;

    Node * it = freeList.head;
    Node * itPrev = nullptr;
    while (it != nullptr) {
        if (ptr < it) {
            freeList.insert(itPrev, freeNode);
            break;
        }
        itPrev = it;
        it = it->next;
    }
    
    used -= freeNode->data.blockSize;

    Coalescence(itPrev, freeNode);  
}

void FreeListAllocator::Coalescence(Node* previousNode, Node * freeNode) {   
    if (freeNode->next != nullptr && 
            (std::size_t) freeNode + freeNode->data.blockSize == (std::size_t) freeNode->next) {
        freeNode->data.blockSize += freeNode->next->data.blockSize;
        freeList.remove(freeNode->next);
    }
    
    if (previousNode != nullptr &&
            (std::size_t) previousNode + previousNode->data.blockSize == (std::size_t) freeNode) {
        previousNode->data.blockSize += freeNode->data.blockSize;
        freeList.remove(freeNode);
    }
}

void FreeListAllocator::Reset() {
    used = 0;
    peak = 0;
    Node * firstNode = (Node *) start;
    firstNode->data.blockSize = totalSize;
    firstNode->next = nullptr;
    freeList.head = nullptr;
    freeList.insert(nullptr, firstNode);
}