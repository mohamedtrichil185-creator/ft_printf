/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 17:34:27 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/01 15:40:14 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include "libft.h"
#include <stdarg.h>

typedef struct s_flags
{
	int	plus;
	int	mines;
	int	hachtag;
	int	zero;
	int	space;

	int	width;
	int	doth_number;
	int	conversion;

	int	number;
}	t_flags;

int	ft_printf(const char *format, ...);

int take_format(va_list ap, const char *format);

void	print_c(char c);
void	print_x(unsigned long n);
void	print_X(unsigned long n);
void	print_d_i(int n);
void	print_p(void *n);
void	print_u(unsigned int n);
void	print_s(const char *s);

#endif
