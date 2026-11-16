/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 10:07:05 by mtraichl          #+#    #+#             */
/*   Updated: 2026/10/31 10:27:14 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int    print_x1(int *count, unsigned long n, int size)
{
    int count;

    count = 0;
    if (n > 15)
        count = print_x1(n / 16);
    put_len(count, &"0123456789abcdef"[n % 16], 1);
    return (count);
}

int	print_p(void *n)
{
	int	count;

	count = 0;
	if (n == NULL)
	{
		put_len(&count, "(nil)", 5);
		return (count);
	}
	put_len(&count, "0x", 2);
	count += print_x1((unsigned long)n);
	return (count);
}
