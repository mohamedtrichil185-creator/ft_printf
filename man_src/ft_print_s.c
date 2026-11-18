/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_s.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:15:29 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:41:29 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_s(const char *s)
{
	int	count;

	count = 0;
	if (!s)
	{
		put_len(&count, "(null)", 6);
		return (count);
	}
	put_len(&count, s, ft_strlen(s));
	return (count);
}
