#include "malloc.h"

void *allocate_memory()

void *malloc_init(size_t size){
    void *ptr;
    if (size <= 0) {
        return NULL
    }
    else if (size <= TINY_SIZE){
        ptr = allocate_memory(size, "tinny");
    }
    return (ptr)
}

void *ft_malloc(size_t size){
    void *ptr;

    pthread_mutex_lock(&global_mutex);
    ptr = malloc_init(size);
    pthread_mutex_unlock(&global_mutex);
    return (ptr);
}