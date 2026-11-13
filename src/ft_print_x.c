/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 19:00:35 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/11 12:24:45 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int    print_x(t_flags *p,unsigned long n)
{
    int count;

    count = 0;
    if (n > 15)
        count += print_x(p, n / 16);
    count += write(1, &"0123456789abcdef"[n % 16], 1);
    return (count);
}
