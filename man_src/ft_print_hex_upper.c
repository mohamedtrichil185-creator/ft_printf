/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_upper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 19:00:58 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:36:36 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_hex_upper(unsigned int n)
{
	int	count;
	int	tmp;

	tmp = 0;
	count = 0;
	if (n > 15)
	{
		count += print_hex_upper(n / 16);
		if (count < 0)
			return (-1);
	}
	tmp = write(1, &"0123456789ABCDEF"[n % 16], 1);
	if (tmp < 0)
		return (-1);
	count += tmp;
	return (count);
}
