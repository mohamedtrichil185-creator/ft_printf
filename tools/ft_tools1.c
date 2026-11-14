#include "ft_printf.h"

void    get_perfix_signe_and_p_len(t_flags *p, t_man **m, int n)
{
    if (n < 0)
    {
        (*m)->perfix_type[0] = '-';
        (*m)->perfix_len = 1;
    }
    else if (p->plus)
    {
        (*m)->perfix_type[0] = '+';
        (*m)->perfix_len = 1;
    }
    else if (p->space)
    {
        (*m)->perfix_type[0] = ' ';
        (*m)->perfix_len = 1;
    }
}

int get_len_of_number(int n)
{
    int count;

    count = 0;
	if (n == 0)
		count++;
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

t_flags	*clean_flags(t_flags *f)
{
	if (f->mines || f->dot)
		f->zero = 0;
	if (f->plus)
		f->space = 0;
	return (f);
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