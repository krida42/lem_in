#include "../include/libft.h"
static size_t count_words(char const *s, char c)
{
    size_t count;
    int in_word;

    count = 0;
    in_word = 0;
    while (*s)
    {
        if (*s != c && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (*s == c)
            in_word = 0;
        s++;
    }
    return (count);
}

static size_t get_word_len(char const *s, char c)
{
    size_t len;

    len = 0;
    while (s[len] && s[len] != c)
        len++;
    return (len);
}

static void free_split(char **split_array)
{
    int i;

    i = 0;
    while (split_array[i])
    {
        free(split_array[i]);
        i++;
    }
    free(split_array);
}

char **ft_split(char const *s, char c)
{
    char **result;
    size_t word_count;
    size_t i;
    size_t word_len;

    if (!s)
        return (NULL);
    word_count = count_words(s, c);
    result = (char **)malloc((word_count + 1) * sizeof(char *));
    if (!result)
        return (NULL);
    i = 0;
    while (i < word_count)
    {
        while (*s == c)
            s++;
        word_len = get_word_len(s, c);
        result[i] = ft_substr(s, 0, word_len);
        if (!result[i])
        {
            free_split(result);
            return (NULL);
        }
        s += word_len;
        i++;
    }
    result[i] = NULL;
    return (result);
}
