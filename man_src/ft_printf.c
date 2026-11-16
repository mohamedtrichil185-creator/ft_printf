#include "ft_printf.h"

void put_len(int *count, const char *s, size_t size)
{
    int ret;

    if (*count < 0)
        return ;
    ret = write(1, s, size);
    if (ret < 0)
        *count = -1;
    else
        *count += ret;
}

int	get_format(va_list ap, const char **s)
{
	if (**s == '\0')
		return (0);
	if (**s == 'c')
		return (print_c(va_arg(ap, int)));
	else if (**s == 'd' || **s == 'i')
		return (print_d_i(va_arg(ap, int)));
	else if (**s == 's')
		return (print_s(va_arg(ap, char *)));
	else if (**s == 'p')
		return (print_p(va_arg(ap, void *)));
	else if (**s == 'u')
		return (print_u(va_arg(ap, unsigned int)));
	else if (**s == 'x')
		return (print_hex_lower(va_arg(ap, unsigned int)));
	else if (**s == 'X')
		return (print_hex_upper(va_arg(ap, unsigned int)));
	else if (**s == '%')
		return (print_c('%'));
	else
		return (print_c(**s));
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;
	va_start(ap, s);
	int	count;
	int	er;

	count = 0;
	er = 0;
	while (*s)
	{
		if (*s == '%')
		{
			s++;
			count += get_format(ap, &s);
			if (!*s)
				break ;
		}
		else
		{
			er = write(1, s, 1);
			if (er < 0)
				return (0);
			count += er;
		}
		s++;
	}
	return (count);
}
