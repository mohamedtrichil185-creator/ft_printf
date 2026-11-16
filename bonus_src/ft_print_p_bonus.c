/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 10:07:05 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/15 04:13:13 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
 
static int    print_x1(unsigned long n)
{
    int count;

    count = 0;
    if (n > 15)
        count += print_x1(n / 16);
    count += write(1, &"0123456789abcdef"[n % 16], 1);
    return (count);
}

static void hanld_NULL(t_flags *p, t_man *m, int *count)
{
    m->src_len = 5;
    set_man(p, &m);
    if (!m->left)
    {
        *count += ft_pad(' ', m->pad);
        *count += write(1, "(nil)", 5);
    }
    else
    {
        *count += write(1, "(nil)", 5);
        *count += ft_pad(' ', m->pad);
    }
}

int get_len_of_number_hex(unsigned long n)
{
    int count;

    count = 0;
    while (n)
    {
        count++;
        n /= 16;
    }
    return (count);
}

int print_p(t_flags *p, void *n, t_man *m)
{
    int  count;
    unsigned long s;

    s = (unsigned long)n;
    count = 0;
    if (n == NULL)
    {
        hanld_NULL(p, m, &count);
        return (count);
    }

    m->src_len = get_len_of_number_hex(s) + 2;
    set_man(p, &m);
    if (m->left)
    {
        count += write(1, "0x", 2);
        count += print_x1(s);
        count += ft_pad(' ', m->pad);
    }
    else
    {
        count += ft_pad(' ', m->pad);
        count += write(1, "0x", 2);
        count += print_x1(s);
    }
    return (count);
}
