/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 10:07:05 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/11 12:20:30 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
 
int print_p(t_flags *p, void *n)
{
    int  count;

    count = 0;
    if (n == NULL)
    {
        write(1, "(nil)", 5);
        return  (0);
    }
    count += write(1, "0x", 2);
    count += print_x(p, (unsigned long)&n);
    return (count);
}
