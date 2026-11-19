/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:35:24 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/19 08:15:01 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	put_len(int *count, const char *s, size_t size)
{
	int	ret;

	if (*count < 0)
		return ;
	ret = write(1, s, size);
	if (ret < 0)
		*count = -1;
	else
		*count += ret;
}

int	get_format(va_list ap, const char **s)
{
	if (**s == '\0')
		return (0);
	if (**s == 'c')
		return (print_c(va_arg(ap, int)));
	else if (**s == 'd' || **s == 'i')
		return (print_d_i(va_arg(ap, int)));
	else if (**s == 's')
		return (print_s(va_arg(ap, char *)));
	else if (**s == 'p')
		return (print_p(va_arg(ap, void *)));
	else if (**s == 'u')
		return (print_u(va_arg(ap, unsigned int)));
	else if (**s == 'x')
		return (print_hex_lower(va_arg(ap, unsigned int)));
	else if (**s == 'X')
		return (print_hex_upper(va_arg(ap, unsigned int)));
	else if (**s == '%')
		return (print_c('%'));
	else
		return (print_c(**s));
}

static void	print_one(int *count, int er, va_list ap, const char *s)
{
	er = get_format(ap, &s);
	if (er < 0)
		*count = -1;
	*count += er;
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;
	int		count;
	int		er;

	er = 0;
	va_start(ap, s);
	count = 0;
	while (*s)
	{
		if (*s == '%')
		{
			s++;
			print_one(&count, er, ap, s);
			if (!*s || count < 0)
				break ;
		}
		else
		{
			if (write(1, s, 1) < 0)
				return (-1);
			count++;
		}
		s++;
	}
	return (count);
}
