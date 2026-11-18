/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_d_i.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:13:13 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:35:54 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	put_nbr_len(int *count, int n)
{
	char	c;

	if (n == -2147483648)
	{
		put_len(count, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		n = -n;
		put_len(count, "-", 1);
	}
	if (n >= 10)
		put_nbr_len(count, n / 10);
	c = n % 10 + '0';
	put_len(count, &c, 1);
}

int	print_d_i(int n)
{
	int	count;

	count = 0;
	put_nbr_len(&count, n);
	return (count);
}
