#include <stdio.h>
#include "ft_printf.h"

int	main()
{

	ft_printf("[%05.3d]", 44);
	printf("[%05.3d]", 44);
	//printf("sizeof printf   : %d\n", printf("[%-5.3s]", "moha"));
	return 0;
}
