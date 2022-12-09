#include "malloc.h"

void		fill_fit(size_t size, t_header **add)
{
    t_header	*tmp;

    tmp = *add;
    tmp->ptr = tmp + 1;
    tmp->free = 0;
    tmp->size = size;
}

t_header	*find_fit(t_header **list, size_t size)
{
//  поиск зафришенной подходящий по размерам зоны
    t_header *tmp;

    tmp = *list;
    while (tmp)
    {
        if (tmp->free && tmp->size == size && tmp->next)
            return (tmp);
        tmp = tmp->next;
    }
    return (NULL);
}

void *allocate_memory_tinny(size_t size){
    t_header *ptr;

    if ((ptr = find_fit(&(env.tiny), size))){
        fill_fit(size, &ptr);
        return (ptr->ptr);
    }
    if (!(ptr = find_free_chunk(&(g_env.tiny), size + sizeof(t_header))))
    {
        ptr = map_tiny_chunk(TINY_ZONE);
        ptr->size = TINY_ZONE - sizeof(t_header);
        fill_info(size, &ptr);
        return (g_env.tiny->ptr);
    }
    return (ptr->ptr);
}

void *malloc_init(size_t size){
//    распределение зон хранения
    void *ptr;
    if (size <= 0) {
        return NULL
    }
    else if (size <= TINY_SIZE){
        ptr = allocate_memory(size);
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