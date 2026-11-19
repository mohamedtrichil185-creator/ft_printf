/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 10:07:05 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:28:38 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

static void	handl_left_null(int er, int *count, t_man *m)
{
	er = ft_pad(' ', m->pad);
	if (er < 0)
		*count = -1;
	else
		*count += er;
	er = write(1, "(nil)", 5);
	if (er < 0)
		*count = -1;
	else
		*count += er;
}

static void	hanld_null(t_flags *p, t_man *m, int *count, int er)
{
	m->src_len = 5;
	set_man(p, &m);
	if (!m->left)
		handl_left_null(er, count, m);
	else
	{
		er = write(1, "(nil)", 5);
		if (er < 0)
			*count = -1;
		else
			*count += er;
		er = ft_pad(' ', m->pad);
		if (er < 0)
			*count = -1;
		else
			*count += er;
	}
}

int	handl_left_p(int count, int er, t_man *m, unsigned long s)
{
	put_len(&count, "0x", 2);
	er = print_x1(s);
	if (er < 0)
		return (-1);
	count += er;
	er = ft_pad(' ', m->pad);
	if (er < 0)
		return (-1);
	count += er;
	return (count);
}

int	handl_right_p(int er, int count, unsigned long s, t_man *m)
{
	er = ft_pad(' ', m->pad);
	if (er < 0)
		return (-1);
	count += er;
	put_len(&count, "0x", 2);
	er = print_x1(s);
	count += er;
	return (count);
}

int	print_p(t_flags *p, void *n, t_man *m)
{
	int				count;
	unsigned long	s;
	int				er;

	er = 0;
	s = (unsigned long)n;
	count = 0;
	if (n == NULL)
	{
		hanld_null(p, m, &count, er);
		return (count);
	}
	m->src_len = get_len_of_number_hex(s) + 2;
	set_man(p, &m);
	if (m->left)
		count = handl_left_p(count, er, m, s);
	else
		count = handl_right_p(er, count, s, m);
	return (count);
}
