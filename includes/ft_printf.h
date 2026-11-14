/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 17:34:27 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/14 11:20:58 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include "libft.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "ft_printf_bonus.h"

int	ft_printf(const char *format, ...);

int take_format(va_list ap, const char **format);

int	get_conversion(va_list ap, const char *format, t_flags *p, t_man *m);

int	print_c(t_flags *p, char c, t_man *m);
int	print_x(t_flags *p, unsigned long n);
int	print_X(t_flags *p, unsigned long n);
int	print_d_i(t_flags *p, int n, t_man *m);
int	print_p(t_flags *p, void *n, t_man *m);
int	print_u(unsigned int n);
int	print_s(t_flags *p, const char *s, t_man *m);

#endif
