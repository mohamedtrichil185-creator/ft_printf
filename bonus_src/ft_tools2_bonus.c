/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools2_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:30:24 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/19 07:14:48 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

bool	is_flags(char c)
{
	return (c == '-' || c == '+' || c == '0' || c == '#' || c == ' ');
}

static bool	is_width(char c)
{
	return (ft_isdigit(c));
}

static bool	is_doth(char c)
{
	return (c == '.');
}

static bool	is_conversion(char c, char *conversion)
{
	while (*conversion)
	{
		if (c == *conversion)
			return (true);
		conversion++;
	}
	return (false);
}

t_flags	*set_flags(const char **format, t_flags *p)
{
	char	num[7];
	char	num1[7];
	int		i;

	ft_bzero(num, 7);
	ft_bzero(num1, 7);
	i = 0;
	while (!is_conversion(**format, "cspdiuxX%") && **format)
	{
		if (is_flags(**format))
		{
			p = get_flags(*format, p);
			(*format)++;
		}
		if (is_width(**format))
		{
			while (ft_isdigit(**format) && **format && i < 7)
				num[i++] = *(*format)++;
			p->width = ft_atoi(num);
		}
		if (is_doth(**format) && **format)
			handl_dot(format, p, num1);
	}
	return (p);
}
