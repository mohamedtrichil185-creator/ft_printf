NAME = libftprintf.a

CFLAGS = -Wall  -Werror

SRC_LIBFT = $(wildcard libft/*.c)

SRC = $(wildcard src/*.c)

SRC_IN = ft_get_format.c \
	 ft_printf.c 

#SRC = ft_printf.c \
      ft_print_d_i.c \
      ft_print_x.c \
      ft_print_X.c \
      ft_print_p.c \
      ft_print_s.c \
      ft_print_c.c \
      ft_print_u.c \
      ft_print_percent.c \
      ft_get_format.c# 

INC = ft_printf.h

OBJ = $(SRC:.c=.o)

OBJ_IN = $(SRC_IN:.c=.o)

OBJ_LIBFT = $(SRC_LIBFT:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(OBJ_LIBFT) $(OBJ_IN)
	ar rcs $(NAME)  $(OBJ) $(OBJ_LIBFT) $(OBJ_IN)

%.o : %.c $(INC)
	cc $(CFLAGS) -I.  -c $< -o $@

clean :
	rm -f $(OBJ) $(OBJ_LIBFT) $(OBJ_IN)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean
