/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_take_format_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:29:51 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/19 07:57:20 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	print_x1(unsigned long n)
{
	int	count;
	int	er;

	er = 0;
	count = 0;
	if (n > 15)
		count = print_x1(n / 16);
	er = write(1, &"0123456789abcdef"[n % 16], 1);
	if (er < 0)
		return (-1);
	count += er;
	return (count);
}

void	handl_dot(const char **format, t_flags *p, char num1[7])
{
	int	i;

	(*format)++;
	if (ft_isdigit(**format))
	{
		i = 0;
		while (ft_isdigit(**format) && **format)
			num1[i++] = *(*format)++;
		p->doth_number = ft_atoi(num1);
	}
	p->dot = 1;
}

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
		return (-1);
	m = malloc(sizeof(t_man));
	if (!m)
	{
		free(p);
		return (-1);
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
