/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:40:40 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/01 15:31:19 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	va_start(ap, format);
	int	i;
	int	count;
	
	i = 0;
	count = 0;
	while (format[i])
	{
		
		write(1, &format[i], 1);
		if (format[i] == '%')
		{
			i++;
			count += take_format(ap, format + i);
		}
		if (format[i])
			i++;
		count++;
	}
	return (count);
}
