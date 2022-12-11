#include "includes/malloc.h"
#include "libft/libft.h"

void		fill_realloc(t_header *prev, t_header *list, size_t new_size)
{
    prev->next = (void*)prev->ptr + new_size;
    prev->next->size = list->size - (new_size - prev->size);
    prev->next->free = 1;
    prev->next->ptr = prev->next + 1;
    prev->size = new_size;
    prev->next->next = list->next;
}

int			fusion(t_header *list, t_header *ptr, size_t size, size_t new_size)
{
    t_header	*prev;
    size_t		total;
    size_t		tmp;

    prev = NULL;
    total = 0;
    while (list)
    {
        tmp = sizeof(t_header) + list->size;
        total += tmp;
        if (prev && prev == ptr && (ptr->size + list->size) >= new_size
            && total <= size)
        {
            fill_realloc(prev, list, new_size);
            return (1);
        }
        if (total > size)
            total = tmp;
        prev = list;
        list = list->next;
    }
    return (0);
}

int			resize_headers(t_header *ptr, size_t size)
{
    t_header	*list;
    size_t		page;

    list = find_list(ptr);
    if (!list)
        return (0);
    if (ptr->size <= TINY_SIZE)
        page = TINY_SIZE;
    else if (ptr->size <= SMALL_SIZE)
        page = SMALL_SIZE;
    else
        return (0);
    return (fusion(list, ptr, page, size));
}

void		*ft_realloc(void *ptr, size_t size)
{
    t_header	*tmp;
    void		*ret;

    tmp = find_mem_chunk(ptr, 0);
    if (!tmp && ptr)
        return (NULL);
    if (tmp && tmp->size < size && tmp->next && tmp->next->free)
    {
        if (resize_headers(tmp, size))
            return (tmp->ptr);
    }
    if (!(ret = ft_malloc(size)))
        return (NULL);
    if (tmp)
    {
        tmp->free = 1;
        ret = ft_memcpy(ret, tmp->ptr, tmp->size);
    }
    return (ret);
}