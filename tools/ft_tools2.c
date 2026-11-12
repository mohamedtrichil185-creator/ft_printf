#include "ft_printf.h"


bool	is_flags(char  c)
{
	return (c == '-' || c == '+' || c == '0'|| c == '#' || c == ' ');
}

static bool	is_width(char c)
{
	return (ft_isdigit(c));
}

static bool	is_doth(char c)
{
	return (c == '.');
}

static bool	is_conversion(char c, char *conversion)
{
	while (*conversion)
	{
		if (c == *conversion)
			return (true);
		conversion++;
	}
	return (false);
}

t_flags	*set_flags(va_list ap, const char **format, t_flags *p)
{
	char	num[7];
	char	num1[7];
	int	i;

	i = 0;
	while (!is_conversion(**format, "cspdiuxX%") && **format)
	{
		if (is_flags(**format))
		{
			p = get_flags(*format, p);
			(*format)++;
		}
		if(is_width(**format))
		{
			while (ft_isdigit(**format) && **format && i < 7)
				num[i++] = *(*format)++;
			p->width = ft_atoi(num);
			// (*format)--;
		}
		if (is_doth(**format) && **format)
		{
			(*format)++;
			if (ft_isdigit(**format))
			{
				i = 0;
				while (ft_isdigit(**format) && **format)
					num1[i++] = *(*format)++;
				p->doth_number = ft_atoi(num1);
			}
			p->dot = 1;
		}
		// (*format)++;
	}
	return (p);
}
