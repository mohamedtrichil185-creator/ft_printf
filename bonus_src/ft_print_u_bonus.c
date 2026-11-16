/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:53:24 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/15 04:13:25 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_putuni_fd(unsigned int number, int fd)
{
    if (number > 9)
		ft_putuni_fd(number / 10, fd);
	((number % 10) + '0', fd);
}

int    print_u(unsigned int n)
{
    ft_putuni_fd(n, 1);
    return (get_len_of_number(n));
}