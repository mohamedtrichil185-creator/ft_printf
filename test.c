
#include <stdio.h>
#include <limits.h>
#include "ft_printf.h"

int	main(void)
{
	int	ft;
	int	original;

	printf("\n========== BASIC %d ==========\n", 0);
	ft = ft_printf("FT:      [%d]\n", 42);
	original = printf("PRINTF:  [%d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== WIDTH ==========\n");
	ft = ft_printf("FT:      [%5d]\n", 42);
	original = printf("PRINTF:  [%5d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== ZERO FLAG ==========\n");
	ft = ft_printf("FT:      [%05d]\n", -42);
	original = printf("PRINTF:  [%05d]\n", -42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== MINUS FLAG ==========\n");
	ft = ft_printf("FT:      [%-8d]\n", 42);
	original = printf("PRINTF:  [%-8d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== PLUS FLAG ==========\n");
	ft = ft_printf("FT:      [%+8d]\n", 42);
	original = printf("PRINTF:  [%+8d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== SPACE FLAG ==========\n");
	ft = ft_printf("FT:      [% 8d]\n", 42);
	original = printf("PRINTF:  [% 8d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== PRECISION ==========\n");
	ft = ft_printf("FT:      [%.5d]\n", 42);
	original = printf("PRINTF:  [%.5d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== PRECISION + ZERO ==========\n");
	ft = ft_printf("FT:      [%08.5d]\n", 42);
	original = printf("PRINTF:  [%08.5d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== PRECISION + PLUS ==========\n");
	ft = ft_printf("FT:      [%+8.5d]\n", 42);
	original = printf("PRINTF:  [%+8.5d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== MINUS + ZERO ==========\n");
	ft = ft_printf("FT:      [%-08d]\n", 42);
	original = printf("PRINTF:  [%-08d]\n", 42);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== ZERO WITH PRECISION ZERO ==========\n");
	ft = ft_printf("FT:      [%.0d]\n", 0);
	original = printf("PRINTF:  [%.0d]\n", 0);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	printf("========== INT LIMITS ==========\n");
	ft = ft_printf("FT:      [%d]\n", INT_MAX);
	original = printf("PRINTF:  [%d]\n", INT_MAX);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	ft = ft_printf("FT:      [%d]\n", INT_MIN);
	original = printf("PRINTF:  [%d]\n", INT_MIN);
	printf("Returns: ft=%d | printf=%d\n\n", ft, original);

	return (0);
}

