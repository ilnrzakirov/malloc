#include "./includes/malloc.h"

void			print_num(unsigned int n)
{
    if (n > 9)
    {
        print_num(n / 10);
        print_num(n % 10);
    }
    else
        ft_putchar_fd(n + '0', STDOUT_FILENO);
}

void		put_hexa(unsigned long h)
{
    const char *hex = "0123456789abcdef";

    if (h >= 16)
    {
        put_hexa(h / 16);
        put_hexa(h % 16);
    }
    else
        ft_putchar_fd(hex[h], STDOUT_FILENO);
}

void		put_pointer(void *ptr)
{
    ft_putstr_fd("0x", STDOUT_FILENO);
    put_hexa((unsigned long)ptr);
}


size_t		print_memory(char *str, t_header *list)
{
    size_t		total;

    total = 0;
    if (list)
    {
        ft_putstr_fd(str, STDOUT_FILENO);
        put_pointer(list);
        ft_putchar_fd('\n', STDOUT_FILENO);
    }
    while (list)
    {
        if (list->free == 0)
        {
            put_pointer(list->ptr);
            ft_putstr_fd(" - ", STDOUT_FILENO);
            put_pointer((void*)list->ptr + list->size);
            ft_putstr_fd(" : ", STDOUT_FILENO);
            print_num(list->size);
            ft_putendl_fd(" octets", STDOUT_FILENO);
            total += list->size;
        }
        list = list->next;
    }
    return (total);
}

void		show_alloc_mem(void)
{
    size_t		total;

    pthread_mutex_lock(&global_mutex);
    total = 0;
    total += print_memory("TINY : ", env.tiny);
    total += print_memory("SMALL : ", env.small);
    total += print_memory("LARGE : ", env.large);
    ft_putstr_fd("total : ", STDOUT_FILENO);
    print_num(total);
    ft_putstr_fd(" octets\n", STDOUT_FILENO);
    pthread_mutex_unlock(&global_mutex);
}