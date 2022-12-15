#include "includes/malloc.h"

t_header	*get_last_header(t_header **list)
{
    t_header *tmp;

    tmp = *list;
    if (!tmp)
        return (NULL);
    while (tmp->next)
        tmp = tmp->next;
    return (tmp);
}

t_header	*map_small_chunk(size_t zone)
{
    t_header	*last;

    if (!env.small)
    {
        if ((env.small = (t_header*)mmap(0, zone, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0))
            == MAP_FAILED)
            return (NULL);
        return (env.small);
    }
    else
    {
        last = get_last_header(&(env.small));
        if ((last->next = (t_header*)mmap(0, zone, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0))
            == MAP_FAILED)
            return (NULL);
        return (last->next);
    }
    return (NULL);
}

void		fill_info(size_t size, t_header **add)
{
    t_header	*tmp;
    t_header	*tmp2;
    t_header	*next;

    tmp = *add;
    next = tmp->next ? tmp->next : NULL;
    tmp->ptr = tmp + 1;
    tmp->free = 0;
    tmp->next = (void*)tmp->ptr + size;
    tmp2 = tmp->next;
    tmp2->ptr = tmp2 + 1;
    tmp2->size = tmp->size - size - sizeof(t_header);
    tmp->size = size;
    tmp2->free = 1;
    tmp2->next = next;
}

t_header	*map_tiny_chunk(size_t zone)
{
//    mmap - отражает адреса физической памяти в вируальной адресной пространстве
//    возвращяет указатель на отображенную область памяти
    t_header	*last;

    if (!env.tiny)
    {
        if ((env.tiny = (t_header*)mmap(0, zone, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0))
            == MAP_FAILED)
            return (NULL);
        return (env.tiny);
    }
    else
    {
        last = get_last_header(&(env.tiny));
        if ((last->next = (t_header*)mmap(0, zone, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0))
            == MAP_FAILED)
            return (NULL);
        return (last->next);
    }
    return (NULL);
}

t_header	*find_free_chunk(t_header **list, size_t size)
{
//    поиск зафришенной зоны
    t_header *tmp;

    tmp = *list;
    while (tmp)
    {
        if (tmp->free && tmp->size >= size)
            return (tmp);
        tmp = tmp->next;
    }
    return (NULL);
}

void		fill_fit(size_t size, t_header **add)
{
    t_header	*tmp;

    tmp = *add;
    tmp->ptr = tmp + 1;
    tmp->free = 0;
    tmp->size = size;
}

void		*allocate_large(size_t size)
{
    t_header	*ptr;

    if (!(ptr = find_free_chunk(&(env.large), size)))
    {
        ptr = get_last_header(&(env.large));
        if (ptr)
        {
            if ((ptr->next = (t_header*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0))
                == MAP_FAILED)
                return (NULL);
            ptr = ptr->next;
        }
        else
        {
            if ((env.large = (t_header*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0))
                == MAP_FAILED)
                return (NULL);
            ptr = env.large;
        }
    }
    ptr->ptr = ptr + 1;
    ptr->size = size;
    ptr->free = 0;
    ptr->next = NULL;
    return (ptr->ptr);
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
    if (!(ptr = find_free_chunk(&(env.tiny), size + sizeof(t_header))))
    {
        ptr = map_tiny_chunk(TINY_ZONE);
        ptr->size = TINY_ZONE - sizeof(t_header);
        fill_info(size, &ptr);
        return (env.tiny->ptr);
    }
    return (ptr->ptr);
}

void		*allocate_small(size_t size)
{
    t_header	*ptr;

    if ((ptr = find_fit(&(env.small), size)))
    {
        fill_fit(size, &ptr);
        return (ptr->ptr);
    }
    if (!(ptr = find_free_chunk(&(env.small), size + sizeof(t_header))))
    {
        ptr = map_small_chunk(SMALL_ZONE);
        ptr->size = SMALL_ZONE - sizeof(t_header);
        fill_info(size, &ptr);
        return (env.small->ptr);
    }
    fill_info(size, &ptr);
    return (ptr->ptr);
}

void *malloc_init(size_t size){
//    распределение зон хранения
    void *ptr;
    if (size <= 0) {
        return NULL;
    }
    else if (size <= TINY_SIZE){
        ptr = allocate_memory_tinny(size);
    }
    else if (size <= SMALL_SIZE) {
        ptr = allocate_small(size);
    }
    else
        ptr = allocate_large(size + sizeof(t_header));
    if (ptr == MAP_FAILED)
        return (NULL);
    return (ptr);
}

void *ft_malloc(size_t size){
    void *ptr;
//  Мютекс для использования в многопоточных приложениях (бонус)
    pthread_mutex_lock(&global_mutex);
    ptr = malloc_init(size);
    pthread_mutex_unlock(&global_mutex);
    return (ptr);
}