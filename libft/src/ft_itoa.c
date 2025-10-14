#include "../include/libft.h"
static size_t count_digits(long n)
{
    size_t count;

    count = 0;
    if (n == 0)
        return (1);
    if (n < 0)
    {
        count++;
        n = -n;
    }
    while (n > 0)
    {
        n /= 10;
        count++;
    }
    return (count);
}

char *ft_itoa(int n)
{
    long num;
    size_t len;
    char *str;

    num = n;
    len = count_digits(num);
    str = (char *)malloc((len + 1) * sizeof(char));
    if (!str)
        return (NULL);

    str[len] = '\0';
    len--;

    if (num == 0)
    {
        str[0] = '0';
        return (str);
    }
    if (num < 0)
    {
        str[0] = '-';
        num = -num;
    }
    while (num > 0)
    {
        str[len] = (num % 10) + '0';
        num /= 10;
        len--;
    }
    return (str);
}
