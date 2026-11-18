/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_upper_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 19:00:58 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:27:58 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static int	print_x1(unsigned int n)
{
	int	count;
	int	er;

	er = 0;
	count = 0;
	if (n > 15)
	{
		er = print_x1(n / 16);
		if (er < 0)
			return (-1);
		count += er;
	}
	er = write(1, &"0123456789ABCDEF"[n % 16], 1);
	if (er < 0)
		return (-1);
	count += er;
	return (count);
}

int	print_hex_upper(t_flags *p, unsigned int n, t_man *m)
{
	int	count;
	int	er;

	er = 0;
	count = 0;
	get_perfix_hex_upper(p, &m, n);
	m->src_len = get_len_of_number_hex(n);
	if (!n && p->width && p->dot && !p->doth_number)
		m->src_len = 0;
	else if (!n && p->dot && !p->doth_number)
		return (0);
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
		count += er;
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
		if (!n && p->dot && !p->doth_number && p->width && !m->perfix_len)
			return (count);
		er = print_x1(n);
		if (er < 0)
			return (-1);
		count += er;
	}
	return (count);
}
