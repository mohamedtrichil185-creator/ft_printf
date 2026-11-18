/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ini_structs_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:24:19 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:26:40 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

t_flags	*ini_flags(t_flags *p)
{
	p->dot = 0;
	p->doth_number = 0;
	p->hash = 0;
	p->mines = 0;
	p->width = 0;
	p->plus = 0;
	p->space = 0;
	p->zero = 0;
	return (p);
}

t_man	*ini_man(t_man *p)
{
	p->left = 0;
	p->pad = 0;
	p->perfix_len = 0;
	p->perfix_type[0] = 0;
	p->perfix_type[1] = 0;
	p->perfix_type[2] = 0;
	p->src_len = 0;
	p->zeros = 0;
	return (p);
}
