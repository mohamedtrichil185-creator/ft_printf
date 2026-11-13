/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 17:34:27 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/13 10:59:28 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include "libft.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct s_flags
{
	int	plus;
	int	mines;
	int	hash;
	int	zero;
	int	space;
	int	doth_number;
	int	dot;
	int	width;
}	t_flags;

typedef struct s_man
{
	int	zeros;
	int	perfix_len;
	char perfix_type[3];
	int	pad;
	int	src_len;
	int left;		
} t_man;

bool	is_flags(char  c);

int	ft_printf(const char *format, ...);

int take_format(va_list ap, const char **format);

t_flags	*set_flags(va_list ap, const char **format, t_flags *p);
t_flags	*get_flags(const char *c,t_flags *p);
t_flags	*ini_struct(t_flags *p);
t_flags	*ini_flags(t_flags *p);

int	get_conversion(va_list ap, const char *format, t_flags *p, t_man *m);
int get_len_of_number(int n);
int ft_pad(int c, int size);

int	print_c(t_flags *p, char c, t_man *m);
int	print_x(t_flags *p, unsigned long n);
int	print_X(t_flags *p, unsigned long n);
int	print_d_i(t_flags *p, int n, t_man *m);
int	print_p(t_flags *p, void *n, t_man *m);
int	print_u(t_flags *p, unsigned int n);
int	print_s(t_flags *p, const char *s, t_man *m);

t_man	*ini_man(t_man *p);

void    set_man(t_flags *p, t_man **m);
void    get_perfix_signe_and_p_len(t_flags *p, t_man **m, int n);
void    get_perfix_0x(t_flags *p, t_man **m,unsigned long n);
void    get_perfix_0X(t_flags *p, t_man **m, unsigned long n);
#endif
