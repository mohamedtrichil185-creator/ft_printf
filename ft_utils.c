/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:28:22 by mtraichl          #+#    #+#             */
/*   Updated: 2026/10/18 13:35:58 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int     ft_find_str(const char *s, char *s_to_find)
{
        int     i;
        int     j;

        i = 0;
        if (!s_to_find)
                return (0);
        while (s[i])
        {
                j = 0;
                if (s[i] == s_to_find[j])
                {
                        while (s[i] && s_to_find[j] && s_to_find[j] == s[i])
                        {
                                j++;
                                i++;
                        }
                }
                if (!s_to_find[j])
                        return (1);
		i++;
	}
	return (0);
}

int     ft_count_var(const char *s, char *s_find)
{
        int     s_find_len;
        int     count;
	char	*p;
	int	i;

	i = 0;
        count = 0;
        s_find_len = ft_strlen(s_find);
	p = ft_strdup(s);
	if (!p)
		return (0);
        while (*p)
        {
                if (ft_find_str(s, s_find))
                {
                        count++;
                        p = ft_strnstr(p, s_find, s_find_len) + s_find_len;
                }
                p++;
		
        }
        return (count);
}


e_type	get_format(char c)
{
	switch (c)
	{
		case 'd':
			return (digit);
		case 'c':
			return (charachter);
		case 's':
			return (string);
		case 'p':
			return (void_ptr);
		case 'i':
			return (digit);
		case 'u':
			return (unsigned_digit);
		case 'x':
			return (hex_lower);
		case 'X':
			return (hex_upper);
		case '%':
			return (percent_sign);
		default :
			return (nothing);
	}
}


/* %d function */

void	print_digit(int number)
{
	ft_putnbr_fd(number, 1);
}


void	print_char(char c)
{
	ft_putchar_fd(c, 1);
}

void	print_string(char *s)
{
	ft_putstr_fd(s, 1);
}

unsigned long size_of_num(unsigned long n)
{
	int	count;

	if (n <= 0)
	{
		count++;
		n = -n;
	}
	count = 0;
	while (n)
	{
		count++;
		n /= 16;
	}
	return (count);
}

void	print_void_ptr(uintptr_t n)
{
	unsigned char *p;
	int			i;

	p = (unsigned char *)&n;
	i = 0;
	write (1, "0x", 2);
	while (i < sizeof(n))
	{
		write (1, &"0123456789ABCDEF"[(p[i] & 0xF0) >> 4], 1);
		write (1, &"0123456789ABCDEF"[p[i] & 0x0F], 1);
		i++;
	}
	write (1, "\n", 1);
}
