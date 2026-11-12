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

void    get_perfix_0x(t_flags *p, t_man **m,unsigned long n)
{
    if (p->hash && n)
    {
        (*m)->perfix_type[0] = '0';
        (*m)->perfix_type[1] = 'x';
        (*m)->perfix_len = 2;
    }
}

void    get_perfix_0X(t_flags *p, t_man **m, unsigned long n)
{
    if (p->hash && n)
    {
        (*m)->perfix_type[0] = '0';
        (*m)->perfix_type[1] = 'X';
        (*m)->perfix_len = 2;
    }
}

void    get_zeros(t_man **m, t_flags *p)
{
    if (p->dot)
    {
        if (!p->doth_number)
            p->doth_number = 0;
        else if (p->doth_number > (*m)->src_len)
        {
            (*m)->zeros = p->doth_number - (*m)->src_len;
            if (p->width >= (p->doth_number + (*m)->perfix_len))
                (*m)->pad = p->width - p->doth_number - (*m)->perfix_len;
        }
    }
    if (p->zero)
    {
        (*m)->zeros += (*m)->pad;
        (*m)->pad = 0;
    }
}

void    get_width(t_man **m, t_flags *p)
{
    int total;

    total = (*m)->perfix_len + (*m)->zeros + (*m)->src_len;
    if (p->width > total)
        (*m)->pad = p->width - total;
    else
        (*m)->pad = 0;
}

void    set_man(t_flags *p, t_man **m)
{
    get_width(m, p);
    get_zeros(m, p);
    if (p->mines)
        (*m)->left = 1;
}