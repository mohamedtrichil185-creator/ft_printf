/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_man_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:29:26 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:29:37 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	get_perfix_hex_lower(t_flags *p, t_man **m, unsigned int n)
{
	if (p->hash && n)
	{
		(*m)->perfix_type[0] = '0';
		(*m)->perfix_type[1] = 'x';
		(*m)->perfix_len = 2;
	}
}

void	get_perfix_hex_upper(t_flags *p, t_man **m, unsigned long n)
{
	if (p->hash && n)
	{
		(*m)->perfix_type[0] = '0';
		(*m)->perfix_type[1] = 'X';
		(*m)->perfix_len = 2;
	}
}

void	get_zeros(t_man **m, t_flags *p)
{
	if (p->dot)
	{
		if (p->doth_number > (*m)->src_len)
		{
			(*m)->zeros = p->doth_number - (*m)->src_len;
			if (p->width >= (p->doth_number + (*m)->perfix_len))
				(*m)->pad = p->width - p->doth_number - (*m)->perfix_len;
		}
	}
	if (p->zero)
	{
		(*m)->zeros += (*m)->pad;
		(*m)->pad = 0;
	}
	if (p->width < ((*m)->perfix_len + (*m)->zeros + (*m)->src_len))
		(*m)->pad = 0;
}

void	get_pad(t_man **m, t_flags *p)
{
	size_t	total;

	total = (*m)->perfix_len + (*m)->zeros + (*m)->src_len;
	if (p->width > total)
		(*m)->pad = p->width - total;
	else
		(*m)->pad = 0;
}

void	set_man(t_flags *p, t_man **m)
{
	get_pad(m, p);
	get_zeros(m, p);
	if (p->mines)
		(*m)->left = 1;
}
