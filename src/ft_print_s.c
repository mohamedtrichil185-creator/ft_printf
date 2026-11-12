/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_s.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 18:15:29 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/11 12:21:48 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int    print_s(t_flags *p, const char *s)
{
    int count;

    count = ft_strlen(s);
    ft_putstr_fd(s, 1);
    return (count);
}
