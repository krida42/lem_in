#include "../include/libft.h"
static int ft_unsigned_len(unsigned int n)
{
    int len = 0;
    if (n == 0)
        return (1);
    while (n > 0)
    {
        len++;
        n /= 10;
    }
    return len;
}

static void ft_putunbr_fd(unsigned int n, int fd)
{
    if (n >= 10)
        ft_putunbr_fd(n / 10, fd);
    ft_putchar_fd((n % 10) + '0', fd);
}

int ft_print_unsigned(unsigned int n)
{
    int len;

    len = ft_unsigned_len(n);
    ft_putunbr_fd(n, 1);
    return (len);
}
