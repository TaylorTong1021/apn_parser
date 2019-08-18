
#include "memory.h"

#include <assert.h>
#define NDEBUG

void* _alloc_memory(unsigned int size) {
    void* ptr = NULL;
    if(size <= 0) {
        assert(0);
    }
    ptr = malloc(size);
    assert(ptr != NULL);
    return ptr;
}