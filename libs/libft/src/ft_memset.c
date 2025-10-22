#include "../include/libft.h"
void *ft_memset(void *b, int c, size_t len)
{
    unsigned char *ptr;
    unsigned char value;
    size_t i;

    ptr = (unsigned char *)b;
    value = (unsigned char)c;
    i = 0;
    while (i < len)
    {
        ptr[i] = value;
        i++;
    }
    return (b);
}
