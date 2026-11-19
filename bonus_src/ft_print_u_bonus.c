/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_u_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:53:24 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:29:13 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	put_nbr_len_u(int *count, unsigned int n)
{
	char	c;

	if (n >= 10)
		put_nbr_len_u(count, n / 10);
	c = n % 10 + '0';
	put_len(count, &c, 1);
}

int	get_len_of_number_u(unsigned int n)
{
	int	count;

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

static int	handl_left(int count, unsigned int n, t_man *m)
{
	int	tmp;

	tmp = 0;
	count = ft_pad('0', m->zeros);
	if (count < 0)
		return (-1);
	put_nbr_len_u(&count, n);
	if (count < 0)
		return (-1);
	tmp = ft_pad(' ', m->pad);
	if (tmp < 0)
		return (-1);
	count += tmp;
	return (count);
}

static int	handl_right(t_man *m, int count, t_flags *p, unsigned int n)
{
	int	tmp;

	tmp = 0;
	count = ft_pad(' ', m->pad);
	if (count < 0)
		return (-1);
	tmp = ft_pad('0', m->zeros);
	if (tmp < 0)
		return (-1);
	count += tmp;
	if (!n && p->dot && !p->doth_number && p->width && !m->perfix_len)
		return (count);
	put_nbr_len_u(&count, n);
	return (count);
}

int	print_u(t_flags *p, unsigned int n, t_man *m)
{
	int	count;

	count = 0;
	m->src_len = get_len_of_number_u(n);
	if (!n && p->width && p->dot && !p->doth_number)
		m->src_len = 0;
	set_man(p, &m);
	if (!n && p->dot && !p->doth_number && !p->width && !m->perfix_len)
		return (0);
	if (m->left)
		count = handl_left(count, n, m);
	else
		count = handl_right(m, count, p, n);
	return (count);
}
