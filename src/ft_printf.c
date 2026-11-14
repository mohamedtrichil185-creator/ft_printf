/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 16:40:40 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/14 11:43:57 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int ft_printf(const char *format, ...)
{
    va_list ap;
    int     count;

    count = 0;
    va_start(ap, format);
    while (*format)
    {
        if (*format == '%')
        {
            format++;
            count += take_format(ap, &format);
            if (!*format)
                break ;
        }
        else
            count += write(1, format, 1);
        format++;
    }
    va_end(ap);
    return (count);
}
