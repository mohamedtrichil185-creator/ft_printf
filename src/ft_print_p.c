/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 10:07:05 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/13 11:10:42 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
 
static int    print_x1(unsigned long n)
{
    int count;

    count = 0;
    if (n > 15)
        count += print_x1(n / 16);
    count += write(1, &"0123456789abcdef"[n % 16], 1);
    return (count);
}

static void hanld_NULL(t_flags *p, t_man *m, int *count)
{
    m->src_len = 5;
    set_man(p, &m);
    if (!m->left)
    {
        *count += ft_pad(' ', m->pad);
        *count += write(1, "(nil)", 5);
    }
    else
    {
        *count += write(1, "(nil)", 5);
        *count += ft_pad(' ', m->pad);
    }
}

int get_len_of_number_hex(unsigned long n)
{
    int count;

    count = 0;
    while (n)
    {
        count++;
        n /= 16;
    }
    return (count);
}

int print_p(t_flags *p, void *n, t_man *m)
{
    int  count;
    unsigned long s;

    s = (unsigned long)n;
    count = 0;
    if (n == NULL)
    {
        hanld_NULL(p, m, &count);
        return (count);
    }

    m->src_len = get_len_of_number_hex(s) + 2;
    set_man(p, &m);
    if (m->left)
    {
        count += write(1, "0x", 2);
        count += print_x1(s);
        count += ft_pad(' ', m->pad);
    }
    else
    {
        count += ft_pad(' ', m->pad);
        count += write(1, "0x", 2);
        count += print_x1(s);
    }
    return (count);
}

// int    print_p(t_flags *d, void *n)
// {
//     unsigned char *p;
//     int                     i;
//     int                     count;

//     count = 0;
//     if (n == NULL)
//     {
//         write(1, "(nil)", 5);
//         return  (0);
//     }
//     p = (unsigned char *)&n;
//     i = sizeof(n) - 2;
//     count += write (1, "0x", 2);
//     while (i--)
//     {
//         count += write (1, &"0123456789abcdef"[(p[i] & 0xF0) >> 4], 1);
//         count += write (1, &"0123456789abcdef"[p[i] & 0x0F], 1);
//     }
//     return (count);
// }

// #include <unistd.h>

// static int  ft_put_hex(unsigned long n)
// {
//     char    *base;
//     int     count;

//     base = "0123456789abcdef";
//     count = 0;
//     if (n >= 16)
//         count += ft_put_hex(n / 16);
//     count += write(1, &base[n % 16], 1);
//     return (count);
// }

// int print_p(t_flags *p, void *ptr)
// {
//     unsigned long   addr;
//     int             count;

//     addr = (unsigned long)ptr;
//     if (addr == 0)
//         return (write(1, "(nil)", 5));
//     count = write(1, "0x", 2);
//     count += ft_put_hex(addr);
//     return (count);
// }
