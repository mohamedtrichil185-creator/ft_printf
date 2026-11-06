/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:53:24 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/05 15:52:47 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putuni_fd(unsigned int number, int fd)
{
    if (number > 9)
		ft_putuni_fd(number / 10, fd);
	ft_putchar_fd((number % 10) + '0', fd);
}

int    print_u(t_flags *p, unsigned int n)
{
    ft_putuni_fd(n, 1);
    return (1);
}