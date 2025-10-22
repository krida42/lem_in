#include "../include/libft.h"
static int ft_formats(va_list args, const char format)
{
    int print_length;

    print_length = 0;
    if (format == 'c')
        print_length += ft_print_char(va_arg(args, int));
    else if (format == 's')
        print_length += ft_print_str(va_arg(args, char *));
    else if (format == 'p')
        print_length += ft_print_ptr(va_arg(args, unsigned long long));
    else if (format == 'd' || format == 'i')
        print_length += ft_print_nbr(va_arg(args, int));
    else if (format == 'u')
        print_length += ft_print_unsigned(va_arg(args, unsigned int));
    else if (format == 'x' || format == 'X')
        print_length += ft_print_hex(va_arg(args, unsigned int), format);
    else if (format == '%')
        print_length += ft_print_char('%');
    return (print_length);
}

int ft_printf(const char *str, ...)
{
    int i;
    va_list args;
    int print_length;

    i = 0;
    print_length = 0;
    if (!str)
        return (-1);
    va_start(args, str);
    while (str[i])
    {
        if (str[i] == '%')
        {
            i++;
            if (str[i] == '\0')
                break;
            print_length += ft_formats(args, str[i]);
        }
        else
            print_length += ft_print_char(str[i]);
        i++;
    }
    va_end(args);
    return (print_length);
}
