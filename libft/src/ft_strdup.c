#include "../include/libft.h"
char *ft_strdup(const char *s1)
{
    size_t len;
    char *dup;

    len = ft_strlen(s1);
    dup = (char *)malloc((len + 1) * sizeof(char));
    if (!dup)
        return (NULL);
    ft_memcpy(dup, s1, len + 1);
    return (dup);
}
