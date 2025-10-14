#include "../include/libft.h"
int ft_print_hex(unsigned int n, const char format)
{
    int len;

    if (n == 0)
    {
        write(1, "0", 1);
        return (1);
    }
    len = ft_hex_len((unsigned long long)n);
    ft_put_hex_digit((unsigned long long)n, format);
    return (len);
}
