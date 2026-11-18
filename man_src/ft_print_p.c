/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>         +#+  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 10:07:05 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:38:15 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_x1(int *count, unsigned long n)
{
	if (n > 15)
		print_x1(count, n / 16);
	put_len(count, &"0123456789abcdef"[n % 16], 1);
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
	print_x1(&count, (unsigned long)n);
	return (count);
}
