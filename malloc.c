#include "malloc.h"

void *allocate_memory(size_t size, char *type){
    t_header *ptr;

    if (ptr = )
}

void *malloc_init(size_t size){
//    распределение зон хранения
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
//  Мютекс для использования в многопоточных приложениях (бонус)
    pthread_mutex_lock(&global_mutex);
    ptr = malloc_init(size);
    pthread_mutex_unlock(&global_mutex);
    return (ptr);
}