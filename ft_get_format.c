#include "ft_printf.h"

void	print_struct(const t_flags *p)
{
	printf("converion : %d\n", p->conversion);
	printf("doth_number : %d\n", p->doth_number);
	printf("hachtag : %d\n", p->hachtag);
	printf("minus : %d\n", p->mines);
	printf("number %d\n", p->number_width);
	printf("pluse : %d\n", p->plus);
	printf("space : %d\n", p->space);
	printf("zero : %d\n", p->zero);
}
// int	do_flags(t_flags *p, format)

int	take_format(va_list ap, const char **format)
{
	t_flags *p;

	p = malloc(sizeof(t_flags));
	if (!p)
		return (0);

	p = ini_struct(p);
	p = set_flags(ap, format, p);

	get_conversion(ap, *format, p);
	// printf("\n\n");
	// print_struct(p);
	return (1);
}
