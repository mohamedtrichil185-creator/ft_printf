#include <stdio.h>
#include "ft_printf.h"

int	main()
{
	int	ft;
	int	original;

	printf("========== ZERO WITH PRECISION ZERO ==========\n");
	ft = ft_printf("FT:      [%.0d]\n", 0);
	original = printf("PRINTF:  [%.0d]\n", 0);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);
	return (0);
}









