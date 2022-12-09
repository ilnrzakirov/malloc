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
    void			*mem;
    size_t			size;
    int				free;
    struct s_header	*next;
}					t_header;

pthread_mutex_t		global_mutex;
t_env env;

#endif