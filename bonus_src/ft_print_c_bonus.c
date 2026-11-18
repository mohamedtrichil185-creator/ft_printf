/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_c_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:13:17 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:27:01 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

int	print_c(t_flags *p, char c, t_man *m)
{
	int	count;
	int	er;

	er = 0;
	count = 0;
	m->src_len = 1;
	set_man(p, &m);
	if (m->left)
	{
		count = ft_pad(c, 1);
		if (count < 0)
			return (-1);
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
		er = ft_pad(c, 1);
		if (er < 0)
			return (-1);
		count += er;
	}
	return (count);
}
