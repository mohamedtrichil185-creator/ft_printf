#include "ft_printf.h"

void	print_struct(const t_flags *p)
{
	printf("dot : %d\n", p->dot);
	printf("doth_number : %d\n", p->doth_number);
	printf("hash : %d\n", p->hash);
	printf("minus : %d\n", p->mines);
	printf("width %d\n", p->width);
	printf("pluse : %d\n", p->plus);
	printf("space : %d\n", p->space);
	printf("zero : %d\n", p->zero);
}
// int	do_flags(t_flags *p, format)


t_flags	*clean_flags(t_flags *f)
{
	if (f->mines || f->dot)
		f->zero = 0;
	if (f->plus)
		f->space = 0;
	return (f);
}

int	take_format(va_list ap, const char **format)
{
	t_flags *p;
	int	count;
	t_man *m;
	
	count = 0;
	m = malloc(sizeof(t_man));
	if (!m)
		return (0);
	p = malloc(sizeof(t_flags));
	if (!p)
		return (0);

	p = ini_flags(p);
	p = set_flags(ap, format, p);
	p = clean_flags(p);

	m = ini_man(m);
	count += get_conversion(ap, *format, p, m);
	// printf("\n\n");
	// print_struct(p);
	return (count);
}
