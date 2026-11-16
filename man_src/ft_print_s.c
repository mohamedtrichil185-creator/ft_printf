/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_s.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:15:29 by mtraichl          #+#    #+#             */
/*   Updated: 2026/10/31 10:27:07 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_s(const char *s)
{
	int	count;

	count = 0;
	if (!s)
	{
		count = write(1, "(null)", 6);
		if (count < 0)
			return (0);
		return (count);
	}
	count = write(1, s, ft_strlen(s));
	if (count < 0)
		return (0);
	return (count);
}
