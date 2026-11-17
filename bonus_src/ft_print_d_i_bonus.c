/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_d_i_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:13:13 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/17 04:22:30 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"


void put_len(int *count, const char *s, size_t size)
{
    int ret;

    if (*count < 0)
        return ;
    ret = write(1, s, size);
    if (ret < 0)
        *count = -1;
    else
        *count += ret;
}

void    put_nbr_len(int *count,long long n)
{
    char c;

    if (n >= 10)
        put_nbr_len(count, n / 10);
    c = n % 10 + '0';
    put_len(count, &c, 1);
}

static int	print_un(unsigned int n)
{
	int	count;

	count = 0;
	put_nbr_len(&count, n);
	return (count);
}

static void    get_num(long long nb, t_man *m, int *count)
{
    if (m->left)
    {
        if (m->perfix_len)
            *count += ft_pad(m->perfix_type[0], 1);
        *count += ft_pad('0', m->zeros);
        print_un(nb);
        *count += ft_pad(' ', m->pad);
    }
    else
    {
        *count += ft_pad(' ', m->pad);
        if (m->perfix_len)
            *count += ft_pad(m->perfix_type[0], 1);
        *count += ft_pad('0', m->zeros);
        print_un(nb);
    }
}

int    print_d_i(t_flags *p, int n, t_man *m)
{
    int count;
    long long nb;

    nb = n;
    count = get_len_of_number(n);
    get_perfix_signe_and_p_len(p, &m, n);
    m->src_len = get_len_of_number(n);
    set_man(p, &m);
    if (nb < 0)
        nb = -nb;
    if (!nb)
    {
        if (!p->dot)
        {
            count = 0;
            put_nbr_len(&count, 0);
            return (count);
        }
        else if (!p->doth_number)
            return (0);
    }
    get_num(nb, m, &count);
    return (count);
}
