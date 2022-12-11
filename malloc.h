#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <string.h>
# include <stdint.h>
# include <pthread.h>
# include <assert.h>
# include <pthread.h>

# define u8 uint8_t
# define u16 uint16_t
# define STACK_SIZE 32
# define HEAP_SIZE STACK_SIZE * 4
# define HEADER 4

# define TINY_SIZE			(size_t)getpagesize() * 2
# define SMALL_SIZE			(size_t)getpagesize() * 16
# define TINY_ZONE			(size_t)(TINY_SIZE + sizeof(t_header)) * 100
# define SMALL_ZONE			(size_t)(SMALL_SIZE + sizeof(t_header)) * 100

typedef struct virtual_memory {
    u8 stack[STACK_SIZE];
    char **unmapped;
    u8 heap[HEAP_SIZE];

    struct {
        char **data;
        char **bss;
        char *text;
    } data_t;
}   virtual_memory_t;


typedef struct entity {
    u8* ptr;
    u16 size;
} entity_t;

typedef struct		s_env
{
    struct s_header	*tiny;
    struct s_header	*small;
    struct s_header	*large;
} t_env;

typedef struct		s_header
{
    void			*ptr;
    size_t			size;
    int				free;
    struct s_header	*next;
}					t_header;

pthread_mutex_t		global_mutex;
t_env env;

void *ft_malloc(size_t size);
void *malloc_init(size_t size);
void		*allocate_small(size_t size);
void *allocate_memory_tinny(size_t size);
t_header	*find_fit(t_header **list, size_t size);
void		*allocate_large(size_t size);
void		fill_fit(size_t size, t_header **add);
t_header	*find_free_chunk(t_header **list, size_t size);
t_header	*map_tiny_chunk(size_t zone);
void		fill_info(size_t size, t_header **add);
t_header	*map_small_chunk(size_t zone);
t_header	*get_last_header(t_header **list);

void ft_free(void *ptr);
void		join_headers(t_header *ptr);
void		free_large(t_header *ptr);
t_header	*find_list(t_header *ptr);
t_header	*find_mem_chunk(void *ptr, int flag);
t_header	*find_ptr(void *ptr, t_header *list, int flag);
void		free_other(t_header *list, t_header *ptr, size_t size, size_t tmp);


#endif