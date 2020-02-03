#include "FreeListAllocator.h"
#include <iostream>
#include <vector>

//void* operator new (std::size_t size)
//{
//	return flAllocator->allocate(size);
//}
//
//void* operator new[](std::size_t size)
//{
//	return  flAllocator->allocate(size);
//}
//
//void operator delete (void* pointerToDelete)
//{
//	flAllocator->free(pointerToDelete);
//}
//
//void operator delete[](void* arrayToDelete)
//{
//	flAllocator->free(arrayToDelete);
//}
static Allocator* flAllocator = new (std::nothrow) FreeListAllocator(1024, FreeListAllocator::FIND_BEST);

int main(){

	flAllocator->Init();

	int* i = (int*)flAllocator->allocate(sizeof(int)*10);
	*i = 10;
	//std::cout << *i << std::endl;
	*(i + 1) = 11;
	//std::cout << *(i+1) << std::endl;

	flAllocator->free(i);

	system("pause");
    return 0;
}