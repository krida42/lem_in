#include "../include/libft.h"
char *ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
    size_t len;
    char *result_str;
    size_t i;

    if (!s || !f)
        return (NULL);

    len = ft_strlen(s);
    result_str = (char *)malloc((len + 1) * sizeof(char));
    if (!result_str)
        return (NULL);

    i = 0;
    while (i < len)
    {
        result_str[i] = f(i, s[i]);
        i++;
    }
    result_str[i] = '\0';

    return (result_str);
}
