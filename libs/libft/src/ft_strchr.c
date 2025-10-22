#include "../include/libft.h"
char *ft_strchr(const char *s, int c)
{
    unsigned char uc;

    uc = (unsigned char)c;
    while (*s != '\0')
    {
        if (*s == uc)
            return ((char *)s);
        s++;
    }
    if (uc == '\0')
        return ((char *)s);
    return (NULL);
}
