/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_upper_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 19:00:58 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/15 16:03:00 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int    print_hex_upper(t_flags *p, unsigned int n)
{
    int count;

    count = 0;
    if (n > 15)
        count += print_hex_upper(p, n / 16);
    count += write(1, &"0123456789ABCDEF"[n % 16], 1);
    return (count);
}
