/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_d_i.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:13:13 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/14 11:08:26 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void    get_num(long long nb, t_man *m, int *count)
{
    if (m->left)
    {
        if (m->perfix_len)
            *count += ft_pad(m->perfix_type[0], 1);
        *count += ft_pad('0', m->zeros);
        print_u(nb);
        *count += ft_pad(' ', m->pad);
    }
    else
    {
        *count += ft_pad(' ', m->pad);
        if (m->perfix_len)
            *count += ft_pad(m->perfix_type[0], 1);
        *count += ft_pad('0', m->zeros);
        print_u(nb);
    }
}

int    print_d_i(t_flags *p, int n, t_man *m)
{
    int count;
    long long nb;

    nb = n;
    count = get_len_of_number(nb);
    get_perfix_signe_and_p_len(p, &m, n);
    m->src_len = get_len_of_number(n);
    set_man(p, &m);
    if (nb < 0)
        nb = -nb;
    if (!nb && !p->doth_number)
    {
        ft_putnbr_fd(0, 1);
        return (count);
    }
    get_num(nb, m, &count);
    return (count);
}
