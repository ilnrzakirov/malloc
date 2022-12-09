NAME = libft_malloc_$(HOSTTYPE).so

SRC = malloc.c free.c

OBJ = $(SRC:%.c=obj/%.o)

CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra -g

INC = -I includes
LIB = libft/

all: $(NAME)

$(NAME): obj $(OBJ)
	@make -s -C $(LIB)
	@$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJ) $(INC) -L$(LIB) -lft
	@ln -sf $(NAME) libft_malloc.so

obj/%.o: %.c
	$(CXX) $(CXXFLAGS) $(INC) -o $@ -c $< -I$(LIB)

obj:
	mkdir -p obj/

clean:
	@rm -rf obj

fclean: clean
	@make clean -s -C $(LIB)
	@rm -rf $(NAME)

re: fclean all
	@make re -s -C $(LIB)

.PHONY: fclean all clean re test
