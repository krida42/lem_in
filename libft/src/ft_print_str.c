#include "../include/libft.h"
int ft_print_str(char *s)
{
    int len;

    if (s == NULL)
    {
        write(1, "(null)", 6);
        return (6);
    }
    len = ft_strlen(s);
    write(1, s, len);
    return (len);
}
