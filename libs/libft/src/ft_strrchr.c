#include "../include/libft.h"
char *ft_strrchr(const char *s, int c)
{
    unsigned char uc;
    char *last_occurrence;

    uc = (unsigned char)c;
    last_occurrence = NULL;

    while (*s != '\0')
    {
        if (*s == uc)
            last_occurrence = (char *)s;
        s++;
    }
    if (uc == '\0')
        return ((char *)s);

    return (last_occurrence);
}
