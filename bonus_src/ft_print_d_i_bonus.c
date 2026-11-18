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

void	put_len(int *count, const char *s, size_t size)
{
	int	ret;

	if (*count < 0)
		return ;
	ret = write(1, s, size);
	if (ret < 0)
		*count = -1;
	else
		*count += ret;
}

void	put_nbr_len(int *count, long long n)
{
	char	c;

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
	{
		if (m->perfix_len)
		{
			er = ft_pad(m->perfix_type[0], 1);
			if (er < 0)
			{
				*count = -1;
				return ;
			}
			*count += er;
		}
		er = ft_pad('0', m->zeros);
		if (er < 0)
		{
			*count = -1;
			return ;
		}
		*count += er;
		print_un(nb);
		er = ft_pad(' ', m->pad);
		if (er < 0)
		{
			*count = -1;
			return ;
		}
		*count += er;
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

int	print_d_i(t_flags *p, int n, t_man *m)
{
	int			count;
	long long	nb;

	nb = n;
	count = get_len_of_number(n);
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
