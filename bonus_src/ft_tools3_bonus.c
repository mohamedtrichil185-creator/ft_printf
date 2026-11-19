/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools3_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:19:15 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/19 06:53:06 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	put_len(int *count, const char *s, size_t size)
{
	int	ret;

	if (*count < 0)
		return ;
	ret = write(1, s, size);
	if (ret < 0)
		*count = -1;
	*count += ret;
}

void	put_nbr_len(int *count, long long n)
{
	char	c;

	if (n >= 10)
		put_nbr_len(count, n / 10);
	c = n % 10 + '0';
	put_len(count, &c, 1);
}

int	get_len_of_number_hex(unsigned long n)
{
	int	count;

	count = 0;
	if (n == 0)
		count++;
	while (n)
	{
		count++;
		n /= 16;
	}
	return (count);
}

int	print_x_hex(unsigned int n)
{
	int	count;
	int	er;

	er = 0;
	count = 0;
	if (n > 15)
	{
		er = print_x_hex(n / 16);
		if (er < 0)
			return (-1);
		count += er;
	}
	er = write(1, &"0123456789abcdef"[n % 16], 1);
	if (er < 0)
		return (-1);
	count += er;
	return (count);
}

void	get_src_len(t_man *m, const char *s, t_flags *p)
{
	if (!ft_strlen(s))
		m->src_len = 0;
	else if (p->dot)
	{
		if (p->doth_number > ft_strlen(s))
			m->src_len = ft_strlen(s);
		else
		{
			m->src_len = p->doth_number;
		}
	}
	else
		m->src_len = ft_strlen(s);
}
