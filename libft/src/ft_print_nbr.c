#include "../include/libft.h"
static int ft_nbr_len(int n)
{
    int len = 0;
    long num = n;

    if (num == 0)
        return (1);
    if (num < 0)
    {
        len++;
        num = -num;
    }
    while (num > 0)
    {
        len++;
        num /= 10;
    }
    return len;
}

int ft_print_nbr(int n)
{
    int len;

    len = ft_nbr_len(n);
    ft_putnbr_fd(n, 1);
    return (len);
}
