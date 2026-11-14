#include "ft_printf.h"

// void	print_struct(const t_flags *p)
// {
// 	printf("dot : %d\n", p->dot);
// 	printf("doth_number : %d\n", p->doth_number);
// 	printf("hash : %d\n", p->hash);
// 	printf("minus : %d\n", p->mines);
// 	printf("width %d\n", p->width);
// 	printf("pluse : %d\n", p->plus);
// 	printf("space : %d\n", p->space);
// 	printf("zero : %d\n", p->zero);
// }
// int	do_flags(t_flags *p, format)

int	get_conversion(va_list ap, const char *format, t_flags *p, t_man *m)
{
	int	count;

	count = 0;
	if (*format == 'c')
		count += print_c(p, va_arg(ap, int), m);
	else if (*format == 's')
		count += print_s(p, va_arg(ap, char *), m);
	else if (*format == 'd' || *format == 'i')
		count += print_d_i(p, va_arg(ap, int), m);
	else if (*format == 'p')
		count += print_p(p, va_arg(ap, void *), m);
	else if (*format == 'x')
		count += print_x(p, va_arg(ap, unsigned long));
	else if (*format == 'X')
		count += print_X(p, va_arg(ap, unsigned long));
	else if (*format == '%')
		count += print_c(p, '%', m);
	return (count);
}
int take_format(va_list ap, const char **format)
{
    t_flags *p;
    t_man   *m;
    int     count;

    count = 0;
    p = malloc(sizeof(t_flags));
    if (!p)
        return (0);
    m = malloc(sizeof(t_man));
    if (!m)
    {
        free(p);
        return (0);
    }
    p = ini_flags(p);
    p = set_flags(format, p);
    p = clean_flags(p);
    m = ini_man(m);
    if (**format)
        count = get_conversion(ap, *format, p, m);
    free(p);
    free(m);
    return (count);
}
