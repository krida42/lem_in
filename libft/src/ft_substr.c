#include "../include/libft.h"
char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char *sub;
    size_t s_len;
    size_t copy_len;

    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
    {
        sub = (char *)malloc(1 * sizeof(char));
        if (!sub)
            return (NULL);
        sub[0] = '\0';
        return (sub);
    }
    if (s_len - start < len)
        copy_len = s_len - start;
    else
        copy_len = len;
    sub = (char *)malloc((copy_len + 1) * sizeof(char));
    if (!sub)
        return (NULL);
    ft_strlcpy(sub, s + start, copy_len + 1);
    return (sub);
}
