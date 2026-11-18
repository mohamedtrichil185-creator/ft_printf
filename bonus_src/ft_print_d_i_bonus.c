/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_d_i_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:13:13 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:27:18 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static void	handl_perfix(t_man *m, int *count, int *er)
{
	*er = ft_pad(m->perfix_type[0], 1);
	if (*er < 0)
	{
		*count = -1;
		return ;
	}
	*count += *er;
}

static void	handl_left(t_man *m, int *count, int *er, long long nb)
{
	if (m->left)
	{
		if (m->perfix_len)
			handl_perfix(m, count, er);
		*er = ft_pad('0', m->zeros);
		if (*er < 0)
		{
			*count = -1;
			return ;
		}
		*count += *er;
		put_nbr_len(count, nb);
		*er = ft_pad(' ', m->pad);
		if (er < 0)
		{
			*count = -1;
			return ;
		}
		*count += *er;
	}
}

static void	handl_right(t_man *m, int *count, int *er, long long nb)
{
	*er = ft_pad(' ', m->pad);
	if (*er < 0)
	{
		*count = -1;
		return ;
	}
	*count += *er;
	if (m->perfix_len)
	{
		*er = ft_pad(m->perfix_type[0], 1);
		if (*er < 0)
		{
			*count = -1;
			return ;
		}
		*count += *er;
	}
	*er = ft_pad('0', m->zeros);
	if (*er < 0)
	{
		*count = -1;
		return ;
	}
	*count += *er;
	put_nbr_len(count, nb);
}

static void	get_num(long long nb, t_man *m, int *count, t_flags *p)
{
	int	er;

	er = 0;
	if (m->perfix_len && p->dot && !p->doth_number && !nb)
	{
		*count = 0;
		er = ft_pad(m->perfix_type[0], 1);
		if (er < 0)
			*count = -1;
		*count += er;
		return ;
	}
	if (p->dot && p->width && !p->doth_number && !nb)
	{
		*count = 0;
		er = ft_pad(' ', p->width);
		if (er < 0)
			*count = -1;
		*count += er;
		return ;
	}
	if (m->left)
		handl_left(m, count, &er, nb);
	else
		handl_right(m, count, &er, nb);
}

int	print_d_i(t_flags *p, int n, t_man *m)
{
	int			count;
	long long	nb;

	nb = n;
	count = 0;
	get_perfix_signe_and_p_len(p, &m, n);
	m->src_len = get_len_of_number(n);
	set_man(p, &m);
	if (nb < 0)
		nb = -nb;
	if (!nb && p->dot && !p->doth_number && !p->width && !m->perfix_len)
		return (0);
	get_num(nb, m, &count, p);
	return (count);
}
