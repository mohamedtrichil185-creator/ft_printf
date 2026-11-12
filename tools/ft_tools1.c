#include "ft_printf.h"

int get_len_of_number(int n)
{
    int count;

    count = 0;
    while (n)
    {
        count++;
        n /= 10;
    }
    return (count);
}

int ft_pad(int c, int size)
{
    int count;

    count = 0;
    while (size--)
        count += write(1, &c, 1);
    return (count);
}

int	get_conversion(va_list ap, const char *format, t_flags *p, t_man *m)
{
	int	count;

	count = 0;
	if (*format == 'c')
		count += print_c(p, va_arg(ap, int), m);
	else if (*format == 's')
		count += print_s(p, va_arg(ap, char *));
	else if (*format == 'd' || *format == 'i')
		count += print_d_i(p, va_arg(ap, int), m);
	else if (*format == 'p')
		count += print_p(p, va_arg(ap, void *));
	else if (*format == 'x')
		count += print_x(p, va_arg(ap, unsigned long));
	else if (*format == 'X')
		count += print_X(p, va_arg(ap, unsigned long));
	else if (*format == '%')
		count += print_c(p, '%', m);
	else 
		count += print_c(p, va_arg(ap, int), m);
	return (count);
}


t_flags	*get_flags(const char *c,t_flags *p)
{
	while (is_flags(*c))
	{	
		if (*c == '-')
			p->mines = 1;
		else if (*c == '+')
			p->plus = 1;
		else if (*c == '0')
			p->zero = 1;
		else if (*c == '#')
			p->hash = 1;
		else if (*c == ' ')
			p->space = 1;
		c++;
	}
	return (p);
}