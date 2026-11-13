/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_X.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 19:00:58 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/13 10:47:59 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int    print_X(t_flags *p, unsigned long n)
{
    int count;

    count = 0;
    if (n > 15)
        count += print_X(p, n / 16);
    count += write(1, &"0123456789ABCDEF"[n % 16], 1);
    return (count);
}
