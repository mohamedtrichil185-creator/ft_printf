/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:53:24 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/17 00:19:25 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void    put_nbr_len_u(int *count, unsigned int n)
{
    char c;

    if (n >= 10)
        put_nbr_len_u(count, n / 10);
    c = n % 10 + '0';
    put_len(count, &c, 1);
}

int get_len_of_number_u(unsigned int n)
{
    int count;

    count = 0;
	if (n == 0)
		count++;
    while (n)
    {
        count++;
        n /= 10;
    }
    return (count);
}

int    print_u(t_flags *p, unsigned int n, t_man *m)
{
    int count;
    int tmp;

    tmp = 0;
    count = 0;
    m->src_len = get_len_of_number_u(n);
    set_man(p, &m);
    if (m->left)
    {
        count = ft_pad('0', m->zeros);
        if (count < 0)
            return (-1);
        put_nbr_len_u(&count, n);
        if (count < 0)
            return (-1);
        tmp = ft_pad(' ', m->pad);
        if (tmp < 0)
            return (-1);
        count = tmp;
    }
    else
    {
        count = ft_pad(' ', m->pad);
        if (count < 0)
            return (-1);
        tmp = ft_pad('0', m->zeros);
        if (tmp < 0)
            return (-1);
        count += tmp;
        put_nbr_len_u(&count, n);
        if (count < 0)
            return (-1);
    }
    return (count);
}
