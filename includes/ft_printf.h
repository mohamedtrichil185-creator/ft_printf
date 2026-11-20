/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtraichl <mtraichl@student.1337.ma>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 17:34:27 by mtraichl          #+#    #+#             */
/*   Updated: 2026/11/18 17:51:33 by mtraichl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>

int		ft_printf(const char *s, ...);
void	put_len(int *count, const char *s, size_t size);

int		print_c(char c);
int		print_hex_lower(unsigned int n);
int		print_hex_upper(unsigned int n);
int		print_d_i(int n);
int		print_p(void *n);
int		print_u(unsigned int n);
int		print_s(const char *s);

#endif
