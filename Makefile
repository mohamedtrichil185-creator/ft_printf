NAME = libftprintf.a

BONUS_MARK = .bonus

CFLAGS = -Wall -Wextra -Werror

CC = cc

LIBFT = libft/libft.a

LIBFT_INCLUDES = -Ilibft

MAN_INCLUDES = -Iincludes

BONUS_INCLUDES = -Iincludes

INC = libft/libft.h \
      includes/ft_printf.h \
      includes/ft_printf_bonus.h

MAN_SRC = man_src/ft_print_d_i.c \
      man_src/ft_print_hex_lower.c \
      man_src/ft_print_hex_upper.c \
      man_src/ft_print_p.c \
      man_src/ft_print_s.c \
      man_src/ft_print_c.c \
      man_src/ft_print_u.c \
      man_src/ft_printf.c

BONUS_SRC = bonus_src/ft_print_c_bonus.c \
            bonus_src/ft_print_d_i_bonus.c \
            bonus_src/ft_print_p_bonus.c \
            bonus_src/ft_print_s_bonus.c \
            bonus_src/ft_print_u_bonus.c \
            bonus_src/ft_print_hex_lower_bonus.c \
            bonus_src/ft_print_hex_upper_bonus.c \
            bonus_src/ft_ini_structs_bonus.c \
            bonus_src/ft_printf_bonus.c \
            bonus_src/ft_set_man_bonus.c \
            bonus_src/ft_take_format_bonus.c \
            bonus_src/ft_tools1_bonus.c \
            bonus_src/ft_tools2_bonus.c

BONUS_OBJ = $(BONUS_SRC:.c=.o)

MAN_OBJ = $(MAN_SRC:.c=.o)

all : $(NAME)

$(NAME) : $(MAN_OBJ)
	$(MAKE) -C libft
	cp $(LIBFT) $(NAME)
	ar rcs $(NAME) $(MAN_OBJ)

bonus : $(BONUS_MARK)

$(BONUS_MARK) : $(BONUS_OBJ)
	$(MAKE) -C libft
	cp $(LIBFT) $(NAME)
	ar rcs $(NAME) $(BONUS_OBJ)
	touch $(BONUS_MARK)


%.o : %.c $(INC)
	$(CC) -g $(CFLAGS) $(MAN_INCLUDES) $(BONUS_INCLUDES) $(LIBFT_INCLUDES) -c $< -o $@

clean :
	$(MAKE) -C libft clean
	rm -f $(MAN_OBJ) $(BONUS_OBJ)

fclean : clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re : fclean all

.PHONY : clean
