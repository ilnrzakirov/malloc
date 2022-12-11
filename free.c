#include "includes/malloc.h"

void		free_other(t_header *list, t_header *ptr, size_t size, size_t tmp)
{
    t_header	*prev;
    size_t		total;

    prev = NULL;
    total = 0;
    while (list)
    {
        tmp = sizeof(t_header) + list->size;
        total += tmp;
        if (prev && prev == ptr && list->free && total <= size)
        {
            prev->size += list->size + sizeof(t_header);
            prev->next = list->next;
        }
        else if (prev && list == ptr && prev->free && total <= size)
        {
            prev->size += list->size + sizeof(t_header);
            prev->next = list->next;
        }
        else
            prev = list;
        if (total > size)
            total = tmp;
        list = list->next;
    }
}

t_header	*find_ptr(void *ptr, t_header *list, int flag)
{
    t_header *ret;
    t_header *prev;

    ret = NULL;
    prev = NULL;
    while (list)
    {
        if (flag && prev && ptr >= prev->ptr && list->ptr > ptr)
            return (prev->ptr);
        if (ptr == list->ptr)
            return (list->ptr);
        prev = list;
        list = list->next;
    }
    return (ret);
}

t_header	*find_mem_chunk(void *ptr, int flag)
{
    t_header *ret;

    if ((ret = find_ptr(ptr, env.tiny, flag)))
        return (ret - 1);
    if ((ret = find_ptr(ptr, env.small, flag)))
        return (ret - 1);
    if ((ret = find_ptr(ptr, env.large, flag)))
        return (ret - 1);
    return (NULL);
}

t_header	*find_list(t_header *ptr)
{
    if (ptr->size <= TINY_SIZE)
        return (env.tiny);
    else if (ptr->size <= SMALL_SIZE)
        return (env.small);
    else
        return (env.large);
}

void		free_large(t_header *ptr)
{
    t_header	*prev;
    t_header	*list;

    list = env.large;
    prev = NULL;
    while (list)
    {
        if (list == ptr)
        {
            if (prev)
                prev->next = list->next;
            munmap(ptr, ptr->size + sizeof(t_header));
            return ;
        }
        prev = list;
        list = list->next;
    }
}

void		join_headers(t_header *ptr)
{
    t_header	*list;
    size_t		page;

    list = find_list(ptr);
    page = 0;
    if (!list)
        return ;
    if (ptr->size <= TINY_SIZE)
        page = TINY_SIZE;
    else if (ptr->size <= SMALL_SIZE)
        page = SMALL_SIZE;
    if (!page)
        free_large(ptr);
    else
        free_other(list, ptr, page, 0);
}

void ft_free(void *ptr){
    t_header *tmp;

    if (!ptr)
        return ;
    tmp = find_mem_chunk(ptr, 1);
    if (!tmp)
        return ;
    pthread_mutex_lock(&global_mutex);
    tmp->free = 1;
    join_headers(tmp);
    pthread_mutex_unlock(&global_mutex);
}