NAME = libftprintf.a

CFLAGS = -Wall  -Werror

SRC_LIBFT = libft/ft_isalnum.c \
      libft/ft_isalpha.c \
      libft/ft_isascii.c \
      libft/ft_isdigit.c \
      libft/ft_isprint.c \
      libft/ft_memchr.c \
      libft/ft_memcmp.c \
      libft/ft_memcpy.c \
      libft/ft_memmove.c \
      libft/ft_putchar_fd.c \
      libft/ft_putstr_fd.c \
      libft/ft_split.c \
      libft/ft_strchr.c \
      libft/ft_strdup.c \
      libft/ft_striteri.c \
      libft/ft_strlcat.c \
      libft/ft_strlcpy.c \
      libft/ft_strlen.c \
      libft/ft_strncmp.c \
      libft/ft_strnstr.c \
      libft/ft_strrchr.c \
      libft/ft_strtrim.c \
      libft/ft_tolower.c \
      libft/ft_toupper.c \
      libft/ft_atoi.c \
      libft/ft_calloc.c \
      libft/ft_strmapi.c \
      libft/ft_memset.c \
      libft/ft_itoa.c \
      libft/ft_strjoin.c \
      libft/ft_substr.c \
      libft/ft_putendl_fd.c \
      libft/ft_bzero.c \
      libft/ft_lstclear.c \
      libft/ft_lstlast.c \
      libft/ft_lstnew.c \
      libft/ft_lstsize.c \
      libft/ft_lstadd_back.c \
      libft/ft_lstadd_front.c \
      libft/ft_putnbr_fd.c \
      libft/ft_lstdelone.c \
      libft/ft_lstmap.c \
      libft/ft_lstiter.c \


SRC_PRINTF = ft_get_format.c \
	 ft_printf.c 

SRC = src/ft_print_d_i.c \
      src/ft_print_x.c \
      src/ft_print_X.c \
      src/ft_print_p.c \
      src/ft_print_s.c \
      src/ft_print_c.c \
      src/ft_print_u.c \

INC = ft_printf.h \
       libft/libft.h

OBJ = $(SRC:.c=.o)

OBJ_IN = $(SRC_PRINTF:.c=.o)

OBJ_LIBFT = $(SRC_LIBFT:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(OBJ_LIBFT) $(OBJ_IN)
	ar rcs $(NAME)  $(OBJ) $(OBJ_LIBFT) $(OBJ_IN)

%.o : %.c $(INC)
	cc $(CFLAGS) -g -I. -Ilibft -c $< -o $@

clean :
	rm -f $(OBJ) $(OBJ_LIBFT) $(OBJ_IN)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean
