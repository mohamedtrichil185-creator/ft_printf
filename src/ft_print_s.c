/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_s.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:15:29 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/13 15:46:11 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void hanld_NULL(t_flags *p, t_man *m, int *count)
{
    m->src_len = 6;
    if (p->dot && p->doth_number < 6)
        m->src_len = 0;
    set_man(p, &m);
    if (!m->left)
    {
        *count += ft_pad(' ', m->pad);
        if (p->width && p->doth_number)
            *count += ft_pad(' ', m->zeros);
    }
    *count += write(1, "(null)", m->src_len);
    if (m->left)
    {
        *count += ft_pad(' ', m->pad);
        if (p->width && p->doth_number)
            *count += ft_pad(' ', m->zeros);
    }
}

static void    get_src_len(t_man *m, const char *s, t_flags *p)
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

int    print_s(t_flags *p, const char *s, t_man *m)
{
    int count;

    count = 0;
    if (!s)
    {
        hanld_NULL(p, m, &count);
        return (count);
    }
    get_src_len(m, s, p);
    set_man(p, &m);
    if (m->left)
    {
        count += write(1, s, m->src_len);
        count += ft_pad(' ', m->pad);
    }
    else
    {
        count += ft_pad(' ', m->pad);
        if (p->width && p->doth_number)
            count += ft_pad(' ', m->zeros);
        if (p->doth_number > ft_strlen(s))
            count += write(1, s, m->src_len);
        else
            count += write(1, s, m->src_len);
    }
    return (count);
}
