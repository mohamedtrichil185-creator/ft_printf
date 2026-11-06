/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_d_i.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:13:13 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/06 16:56:31 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int handl_space(t_flags *p, int n)
{
    int count;

    count = 0;
    if (p->number_width > get_len_of_number(n))
        count += ft_pad(' ', p->number_width - get_len_of_number(n));
    else
        count += ft_pad(' ', 1);
    if (p->doth_number > get_len_of_number(n))
        count += ft_pad('0', p->doth_number - get_len_of_number(n));
    ft_putnbr_fd(n, 1);
    return (count);
}

int handl_zero(t_flags *p, int n)
{
    int count;

    count = 0;
    if (p->number_width)
    {
        if (p->doth_number < p->number_width)
        {
            count += ft_pad(' ', p->number_width - p->doth_number);
            count += ft_pad('0', p->doth_number - get_len_of_number(n));
        }
        else  
            count += ft_pad('0', p->doth_number - get_len_of_number(n));
    }
    ft_putnbr_fd(n, 1);
    return (count);
}

int    print_d_i(t_flags *p, int n)
{
    int count;

    count = 0;
    if (p->mines && p->number_width >= get_len_of_number(n))
    {
        ft_putnbr_fd(n, 1);
        count += ft_pad(' ', p->number_width - get_len_of_number(n));
    }
    else if (p->plus && n >= 0)
    {
        count += ft_pad('+', 1);
        ft_putnbr_fd(n, 1);
    }
    else if (p->space)
        count += handl_space(p, n);
    else if (p->zero)
        count += handl_zero(p, n);
    else
        ft_putnbr_fd(n, 1);
    return (count + get_len_of_number(n));
}
