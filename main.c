#include <stdio.h>
#include "ft_printf.h"

int	main()
{
	int	a;

	a = 10;
	ft_printf("[%10.3s]\n", (char *)NULL);
	printf("[%10.3s]\n", (char *)NULL);
}
