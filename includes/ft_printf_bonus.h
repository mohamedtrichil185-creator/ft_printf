#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H

#include "ft_printf.h"

typedef struct s_flags
{
	size_t	plus;
	size_t	mines;
	size_t	hash;
	size_t	zero;
	size_t	space;
	size_t	doth_number;
	size_t	dot;
	size_t	width;
}	t_flags;

typedef struct s_man
{
	size_t	zeros;
	size_t	perfix_len;
	char perfix_type[3];
	size_t	pad;
	size_t	src_len;
	size_t left;		
} t_man;


void    set_man(t_flags *p, t_man **m);
void    get_perfix_signe_and_p_len(t_flags *p, t_man **m, int n);
void    get_perfix_0x(t_flags *p, t_man **m,unsigned long n);
void    get_perfix_0X(t_flags *p, t_man **m, unsigned long n);
bool	is_flags(char  c);

t_flags	*set_flags(const char **format, t_flags *p);
t_flags	*get_flags(const char *c,t_flags *p);
t_man	*ini_man(t_man *p);
t_flags	*ini_flags(t_flags *p);

int	ft_pad(int c, int size);
int	get_len_of_number(int n);
t_flags	*clean_flags(t_flags *f);


#endif