#include "ft_printf.h"

bool	is_conversion(char c, char *conversion)
{
	while (*conversion)
	{
		if (c == *conversion)
			return (true);
		conversion++;
	}
	return (false);
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

bool	is_flags(char  c)
{
	return (c == '-' || c == '+' || c == '0'|| c == '#' || c == ' ');
}

bool	is_width(char c)
{
	return (ft_isdigit(c));
}

bool	is_doth(char c)
{
	return (c == '.');
}

t_flags	*set_flags(va_list ap, const char *format, t_flags *p)
{
	char	num[7];
	char	num1[7];
	int	i;

	while (!is_conversion(*format, "cspdiuxX%") && *format)
	{
		if (is_flags(*format))
			p = get_flags(*format, p);
		if(is_width(*format))
		{
			while (ft_isdigit(*format) && *format && i < 7)
				num[i++] = *format++;
			p->number = ft_atoi(num);
		}
		if (is_doth(*format) && *format)
		{
			format++;
			if (ft_isdigit(*format))
			{
				i = 0;
				while (ft_isdigit(*format) && *format)
					num1[i++] = *format++;
				p->doth_number = ft_atoi(num1);
			}
		}
		format++;
	}
	return (p);
}

int	get_conversion(va_list ap, const char *format, const t_flags *p)
{
	if (*format == 'c')
		print_c(va_arg(ap, int));
	else if (*format == 's')
		print_s(va_arg(ap, char *));
	else if (*format == 'd' || *format == 'i')
		print_d_i(va_arg(ap, int));
	else if (*format == 'p')
		print_p(va_arg(ap, unsigned long *));
	else if (*format == 'x')
		print_x(va_arg(ap, unsigned long));
	else if (*format == 'X')
		print_X(va_arg(ap, unsigned long));
	else if (*format == '%')
		ft_putchar_fd('%', 1);
	else 
		ft_putchar_fd(*format, 1);
	return (1);
}

t_flags	*ini_struct(t_flags *p)
{
	p->conversion = 0;
	p->doth_number = 0;
	p->hachtag = 0;
	p->mines = 0;
	p->number = 0;
	p->plus = 0;
	p->space = 0;
	p->width = 0;
	p->zero = 0;
	return (p);
}

void	print_struct(const t_flags *p)
{
	printf("converion : %d\n", p->conversion);
	printf("doth_number : %d\n", p->doth_number);
	printf("hachtag : %d\n", p->hachtag);
	printf("minus : %d\n", p->mines);
	printf("number %d\n", p->number);
	printf("pluse : %d\n", p->plus);
	printf("space : %d\n", p->space);
	printf("width : %d\n", p->width);
	printf("zero : %d\n", p->zero);
}


int	take_format(va_list ap, const char *format)
{
	t_flags *p;

	p = malloc(sizeof(t_flags));
	if (!p)
		return (0);

	p = ini_struct(p);
	p = set_flags(ap, format, p);
	print_struct(p);

	get_conversion(ap, format, p);
	return (1);
}
