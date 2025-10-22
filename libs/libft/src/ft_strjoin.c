#include "../include/libft.h"
char *ft_strjoin(char const *s1, char const *s2)
{
    size_t len1;
    size_t len2;
    char *joined_str;

    if (!s1 || !s2)
        return (NULL);

    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);

    joined_str = (char *)malloc((len1 + len2 + 1) * sizeof(char));
    if (!joined_str)
        return (NULL);

    ft_memcpy(joined_str, s1, len1);
    ft_memcpy(joined_str + len1, s2, len2 + 1);

    return (joined_str);
}
