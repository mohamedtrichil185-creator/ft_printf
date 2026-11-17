/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_lower_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 19:00:35 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/17 03:45:18 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int get_len_of_number_hex(unsigned int n)
{
    int count;

    count = 0;
	if (n == 0)
		count++;
    while (n)
    {
        count++;
        n /= 16;
    }
    return (count);
}

static int    print_x1(unsigned int n)
{
    int count;
    int er;

    er = 0;
    count = 0;
    if (n > 15)
    {
        er = print_x1(n / 16);
        if (er < 0)
            return (-1);
        count += er;
    }
    er = write(1, &"0123456789abcdef"[n % 16], 1);
    if (er < 0)
        return (-1);
    count += er;
    return (count);
}

int    print_hex_lower(t_flags *p, unsigned int n, t_man *m)
{
    int count;
    int er;

    er = 0;
    count = 0;
    get_perfix_hex_lower(p, &m, n);
    m->src_len = get_len_of_number_hex(n);
    set_man(p, &m);
    if (m->left)
    {
        count = ft_pad('0', m->zeros);
        put_len(&count, m->perfix_type, m->perfix_len);
        if (count < 0)
            return (-1);
        er = print_x1(n);
        if (er < 0)
            return (-1);
        er = ft_pad(' ', m->pad);
        if (er < 0)
            return (-1);
        count += er;
    }
    else
    {
        count = ft_pad(' ', m->pad);
        if (count < 0)
            return (-1);
        put_len(&count, m->perfix_type, m->perfix_len);
        if (count < 0)
            return (-1);
        er = ft_pad('0', m->zeros);
        if (er < 0)
            return (-1);
        count += er;
        er = print_x1(n);
        if (er < 0)
            return (-1);
        count += er;
    }
    return (count);
}
