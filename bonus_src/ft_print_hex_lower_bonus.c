/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_lower_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 19:00:35 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:27:39 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static int	handl_not_hash(t_man *m, int *count)
{
	int	er;

	er = 0;
	er = ft_pad('0', m->zeros);
	if (er < 0)
		return (-1);
	*count += er;
	put_len(count, m->perfix_type, m->perfix_len);
	if (*count < 0)
		return (-1);
	return (*count);
}

static int	handl_hash(t_man *m, int *count)
{
	int	er;

	er = 0;
	put_len(count, m->perfix_type, m->perfix_len);
	if (*count < 0)
		return (-1);
	er = ft_pad('0', m->zeros);
	if (er < 0)
		return (-1);
	*count += er;
	return (*count);
}

static int	handl_left(t_man *m, t_flags *p, int *count, unsigned int n)
{
	int	er;

	er = 0;
	if (!p->hash)
	{
		*count = handl_not_hash(m, count);
		if (*count < 0)
			return (-1);
	}
	else
	{
		*count = handl_hash(m, count);
	}
	er = print_x_hex(n);
	if (er < 0)
		return (-1);
	*count += er;
	er = ft_pad(' ', m->pad);
	if (er < 0)
		return (-1);
	*count += er;
	return (*count);
}

static int	handl_right(t_man *m, t_flags *p, int *count, unsigned int n)
{
	int	er;

	er = 0;
	*count = ft_pad(' ', m->pad);
	if (*count < 0)
		return (-1);
	put_len(count, m->perfix_type, m->perfix_len);
	if (*count < 0)
		return (-1);
	er = ft_pad('0', m->zeros);
	if (er < 0)
		return (-1);
	*count += er;
	if (!n && p->dot && !p->doth_number && p->width && !m->perfix_len)
		return (*count);
	er = print_x_hex(n);
	if (er < 0)
		return (-1);
	*count += er;
	return (*count);
}

int	print_hex_lower(t_flags *p, unsigned int n, t_man *m)
{
	int	count;
	int	er;

	er = 0;
	count = 0;
	get_perfix_hex_lower(p, &m, n);
	m->src_len = get_len_of_number_hex(n);
	if (!n && p->width && p->dot && !p->doth_number)
		m->src_len = 0;
	else if (!n && p->dot && !p->doth_number)
		return (0);
	set_man(p, &m);
	if (m->left)
		count = handl_left(m, p, &count, n);
	else
		count = handl_right(m, p, &count, n);
	return (count);
}
