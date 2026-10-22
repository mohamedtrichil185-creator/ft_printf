NAME = libftprintf.a

SRC = $(filter-out main.c, $(wildcard *.c))

OBJS = $(SRC:.c=.o)

SRC_LIBFT = $(wildcard libft/*.c)

OBJS_LIBFT = $(SRC_LIBFT:.c=.o)

all : $(NAME)
	

$(NAME) : $(OBJS) $(OBJS_LIBFT)
	ar rsc $(NAME) $(OBJS) $(OBJS_LIBFT)

%.o : %.c
	gcc  -c $< -o $@
clean : 
	rm -f $(OBJS) $(OBJS_LIBFT)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
