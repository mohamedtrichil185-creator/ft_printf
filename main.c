#include "ft_printf.h"
#include <limits.h>

int	write_len(const char *s, size_t size)
{
	int	count;

	count = 0;
	count += write(1, s, size);
	if (count < 0)
		return (-1);
	return (count);
}

int	main(void)
{
	int	i;

	i = ft_printf("[%-+8.3d|%08.3d|%#10x|% 5d]", 7, 7, 255, 42);
	ft_putnbr_fd(i, 1);
	return (0);
}
