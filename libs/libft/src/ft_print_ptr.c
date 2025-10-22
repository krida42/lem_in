#include "../include/libft.h"
int ft_print_ptr(unsigned long long ptr)
{
    int print_length;

    print_length = 0;
    if (ptr == 0)
        print_length += write(1, "(nil)", 5);
    else
    {
        print_length += write(1, "0x", 2);
        ft_put_hex_digit(ptr, 'p');
        print_length += ft_hex_len(ptr);
    }
    return (print_length);
}
