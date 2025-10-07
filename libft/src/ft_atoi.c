#include "../include/libft.h"
static int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

int ft_atoi(const char *str)
{
    int sign;
    long long result;

    sign = 1;
    result = 0;
    while (is_whitespace(*str))
        str++;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    else if (*str == '+')
        str++;
    while (ft_isdigit(*str))
    {
        if (sign == 1 && (result > LLONG_MAX / 10 ||
                          (result == LLONG_MAX / 10 && (*str - '0') > LLONG_MAX % 10)))
            return ((int)LLONG_MAX);
        if (sign == -1 && (result > -(LLONG_MIN / 10) ||
                           (result == -(LLONG_MIN / 10) && (*str - '0') > -(LLONG_MIN % 10))))
            return ((int)LLONG_MIN);

        result = result * 10 + (*str - '0');
        str++;
    }
    return ((int)(result * sign));
}
