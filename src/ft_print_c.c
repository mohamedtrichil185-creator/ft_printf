/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_c.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:13:17 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/12 14:39:15 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int print_c(t_flags *p, char c, t_man *m)
{
    int  count;

    count = 1;
    m->src_len = 1;
    set_man(p, &m);
    if (m->left)
    {
        ft_putchar_fd(c, 1);
        count += ft_pad(' ', m->pad);
    }
    else
    {
        count += ft_pad(' ', m->pad);
        ft_putchar_fd(c, 1);
    }
    return (count);
}
