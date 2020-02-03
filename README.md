# Memory Allocator in C++ Language
### Data Structures in C++ Homework

## Task 

The library you are implementing in this project must work on a pre-allocated memory block. It must respond to the *alloc \<size\>* and *free \<ptr\>* commands. You need to choose an efficient memory allocation algorithm that works with randomly sized blocks. Validate working with directories (ie, protect yourself against incorrect or double memory freezing). Try working with different algorithms. If you can - demonstrate the usage of the library with standard containers in stl.

## My Implementation

Everyone knows about dynamic memory allocation.  Our programs can ask for dynamic memory usin 'malloc'. Malloc returns an address to a position in memory where we can store our data. Once we're done with that data, we can call 'free' to free the memory and let others processes use it. No serious application could do without the mechanism directly or indirectly. Inevitably a default implementation of the dynamic memory allocators has to be very basic and general to cover the wide spectrum of all possible usage variations. The cost of generality, however, is often a degradation of the time needed for memory allocation/deallocation, or memory allocation overhead.

For this project I've implemented the Free List Memory Allocator: a general purpose allocator that, contrary to the others, doesn't impose any restriction. It allows allocations and deallocations to be done in any order. From my research, this is how the Free List Memory Allocator (Linked List Imlementation) works: 
*As the name says, this implementation uses a Linked List to store, in a sorted manner, the start address of each free contiguous block in memory and its size. When an allocation is requested, it searches in the linked list for a block where the data can fit. Then it removes the element from the linked list and places an allocation header (used on deallocations) right before the data (as we did in the Stack allocator). On deallocations, we get back the allocation header to know the size of the block that we are going to free. Once we free it we insert it into the sorted linked list and we try to merge contiguous memory blocks together creating bigger blocks.*

## Linked list Allocate

When an allocation is requested, we look for a block in memory where our data can fit. This means that we have to iterate our linked list until we find a block that has a size equal or bigger than the size requested (it can store this data plus the allocation header) and remove it from the linked list.

There are two strategies for allocation:
* first-fit: the search stops when the first block where the memory fits is found;
* best-fit: looks for the free memory block of smaller size that can handle our data. 

The latter operation may take more time because is always iterating through all elements but it can reduce fragmentation.

## Linked list Free

We get back the information about the allocation from the header. Then, we iterate the Linked List to intsert the free block in the right position (because it iss sorted by address). Once we've inserted it, we merge contiguous blocks. We can merge in _O(1) because our Linked List is sorted. We only need to look at the previous and next elements in the linked list to see if we can merge this contiguous blocks (Coalescence).

