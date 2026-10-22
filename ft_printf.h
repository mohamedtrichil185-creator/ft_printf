/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 14:20:14 by mtraichl          #+#    #+#             */
/*   Updated: 2026/10/18 13:28:13 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include "libft/libft.h"
#include <stdint.h>

typedef enum	e_typ {charachter, digit, string, void_ptr, unsigned_digit, hex_upper, hex_lower, percent_sign, nothing} e_type;

int	ft_find_str(const char *s, char *s_to_find);
int	ft_count_var(const char *s, char *s_find);
e_type	get_format(char c);
void    print_void_ptr(unsigned long n);

int	ft_printf(const char *format, ...);
#endif
