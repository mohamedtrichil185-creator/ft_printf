/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:53:24 by mtraichl          #+#    #+#             */
/*   Updated: 2026/10/31 10:27:03 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void put_nbr_len(int *count, unsigned int n)
{
    char c;

    if (n >= 10)
        put_nbr_len(count, n / 10);
    c = n % 10 + '0';
    put_len(count, &c, 1);
}

int	print_u(unsigned int n)
{
	int	count;

	count = 0;
	put_nbr_len(&count, n);
	return (count);
}
