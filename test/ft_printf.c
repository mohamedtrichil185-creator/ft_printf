/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:40:40 by mtraichl          #+#    #+#             */
/*   Updated: 2026/10/31 10:27:25 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>


void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void print_c(char c)
{
	ft_putchar_fd(c, 1);
}

int	get_format(va_list ap, const char *format)
{
	if (*format == 'c')
	{
		print_c(va_arg(ap, int));
	}
	return (1);
}


int	ft_printf(const char *format, ...)
{
	va_list	ap;
	va_start(ap, format);
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			count += get_format(ap, format + i);
		}
		else 
			write (1, &format[i], 1);
		if (format[i])
			i++;
		count++;
	}
	return (count);
}

int	main()
{
//	ft_printf("mo %c %c %c" , 'm', ' ', 'k');
	printf("%-2147483647d", 42);
	return (0);
}
