/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_d_i.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:13:13 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/12 17:00:55 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// int handl_space(t_flags *p, int n)
// {
//     int count;

//     count = 0;
//     if (p->number_width > get_len_of_number(n))
//         count += ft_pad(' ', p->number_width - get_len_of_number(n));
//     else
//         count += ft_pad(' ', 1);
//     if (p->doth_number > get_len_of_number(n))
//         count += ft_pad('0', p->doth_number - get_len_of_number(n));
//     ft_putnbr_fd(n, 1);
//     return (count);
// }

// int handl_zero(t_flags *p, int n)
// {
//     int count;

//     count = 0;
//     if (p->number_width)
//     {
//         if (p->doth_number < p->number_width)
//         {
//             count += ft_pad(' ', p->number_width - p->doth_number);
//             count += ft_pad('0', p->doth_number - get_len_of_number(n));
//         }
//         else  
//             count += ft_pad('0', p->doth_number - get_len_of_number(n));
//     }
//     ft_putnbr_fd(n, 1);
//     return (count);
// }

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
    {
        nb = -nb;
    }

    if (!nb && !p->doth_number)
        return (0);
    else if (m->left)
    {
        if (m->perfix_len)
            count += ft_pad(m->perfix_type[0], 1);
        count += ft_pad('0', m->zeros);
        print_u(p, nb);
        count += ft_pad(' ', m->pad);
    }
    else
    {
        count += ft_pad(' ', m->pad);
        if (m->perfix_len)
            count += ft_pad(m->perfix_type[0], 1);
        count += ft_pad('0', m->zeros);
        print_u(p, nb);
    }
    return (count);
}
