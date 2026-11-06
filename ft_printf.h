/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 17:34:27 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/06 19:28:54 by mtraichl         ###   ########.fr       */
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
	int	hachtag;
	int	zero;
	int	space;

	int	doth_number;
	int	conversion;

	int	number_width;
}	t_flags;

int	ft_printf(const char *format, ...);

int take_format(va_list ap, const char **format);

t_flags	*set_flags(va_list ap, const char **format, t_flags *p);
t_flags	*get_flags(char c,t_flags *p);
t_flags	*ini_struct(t_flags *p);

int	get_conversion(va_list ap, const char *format, t_flags *p);
int get_len_of_number(int n);
int ft_pad(int c, int size);

int	print_c(t_flags *p, char c);
int	print_x(t_flags *p, unsigned long n);
int	print_X(t_flags *p, unsigned long n);
int	print_d_i(t_flags *p, int n);
int	print_p(t_flags *p, void *n);
int	print_u(t_flags *p, unsigned int n);
int	print_s(t_flags *p, const char *s);

#endif
