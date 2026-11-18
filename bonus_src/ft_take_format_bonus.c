/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_take_format_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:29:51 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:30:04 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	get_conversion(va_list ap, const char *format, t_flags *p, t_man *m)
{
	int	count;

	count = 0;
	if (*format == 'c')
		count += print_c(p, va_arg(ap, int), m);
	else if (*format == 's')
		count += print_s(p, va_arg(ap, char *), m);
	else if (*format == 'd' || *format == 'i')
		count += print_d_i(p, va_arg(ap, int), m);
	else if (*format == 'p')
		count += print_p(p, va_arg(ap, void *), m);
	else if (*format == 'x')
		count += print_hex_lower(p, va_arg(ap, unsigned int), m);
	else if (*format == 'X')
		count += print_hex_upper(p, va_arg(ap, unsigned int), m);
	else if (*format == 'u')
		count += print_u(p, va_arg(ap, unsigned int), m);
	else if (*format == '%')
		count += print_c(p, '%', m);
	return (count);
}

int	take_format(va_list ap, const char **format)
{
	t_flags	*p;
	t_man	*m;
	int		count;

	count = 0;
	p = malloc(sizeof(t_flags));
	if (!p)
		return (0);
	m = malloc(sizeof(t_man));
	if (!m)
	{
		free(p);
		return (0);
	}
	p = ini_flags(p);
	p = set_flags(format, p);
	p = clean_flags(p);
	m = ini_man(m);
	if (**format)
		count = get_conversion(ap, *format, p, m);
	free(p);
	free(m);
	return (count);
}
