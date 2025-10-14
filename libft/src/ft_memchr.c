#include "../include/libft.h"
void *ft_memchr(const void *s, int c, size_t n)
{
    const unsigned char *ptr;
    unsigned char uc;
    size_t i;

    ptr = (const unsigned char *)s;
    uc = (unsigned char)c;
    i = 0;
    while (i < n)
    {
        if (ptr[i] == uc)
            return ((void *)(ptr + i));
        i++;
    }
    return (NULL);
}
