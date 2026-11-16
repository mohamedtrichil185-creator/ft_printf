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

int	ft_putuni_fd(unsigned int number, int fd)
{
	int	count;

	count = 0;
	if (number > 9)
		count += ft_putuni_fd(number / 10, fd);
	ft_putchar_fd((number % 10) + '0', fd);
	count++;
	return (count);
}

int	print_u(unsigned int n)
{
	return (ft_putuni_fd(n, 1));
}
