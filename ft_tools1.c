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

t_flags	*ini_struct(t_flags *p)
{
	p->conversion = 0;
	p->doth_number = 0;
	p->hachtag = 0;
	p->mines = 0;
	p->number_width = 0;
	p->plus = 0;
	p->space = 0;
	p->zero = 0;
	return (p);
}

int	get_conversion(va_list ap, const char *format, t_flags *p)
{
	int	count;

	count = 0;
	if (*format == 'c')
		count += print_c(p, va_arg(ap, int));
	else if (*format == 's')
		count += print_s(p, va_arg(ap, char *));
	else if (*format == 'd' || *format == 'i')
		count += print_d_i(p, va_arg(ap, int));
	else if (*format == 'p')
		count += print_p(p, va_arg(ap, void *));
	else if (*format == 'x')
		count += print_x(p, va_arg(ap, unsigned long));
	else if (*format == 'X')
		count += print_X(p, va_arg(ap, unsigned long));
	else if (*format == '%')
		count += print_c(p, '%');
	else 
		count += print_c(p, va_arg(ap, int));
	return (count);
}

t_flags	*get_flags(char c,t_flags *p)
{
	if (c == '-')
		p->mines = 1;
	else if (c == '+')
		p->plus = 1;
	else if (c == '0')
		p->zero = 1;
	else if (c == '#')
		p->hachtag = 1;
	else if (c == ' ')
		p->space = 1;
	else 
		return (p);
	return (p);
}