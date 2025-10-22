#include "../include/libft.h"
void ft_put_hex_digit(unsigned long long n, const char format)
{
    if (n >= 16)
    {
        ft_put_hex_digit(n / 16, format);
        ft_put_hex_digit(n % 16, format);
    }
    else
    {
        if (n < 10)
            ft_putchar_fd((n + '0'), 1);
        else
        {
            if (format == 'x' || format == 'p')
                ft_putchar_fd((n - 10 + 'a'), 1);
            if (format == 'X')
                ft_putchar_fd((n - 10 + 'A'), 1);
        }
    }
}

int ft_hex_len(unsigned long long n)
{
    int len = 0;
    if (n == 0)
        return (1);
    while (n != 0)
    {
        len++;
        n = n / 16;
    }
    return len;
}
