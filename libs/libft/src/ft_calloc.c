#include "../include/libft.h"
void *ft_calloc(size_t nmemb, size_t size)
{
    void *ptr;
    size_t total_size;

    if (nmemb == 0 || size == 0)
    {
        nmemb = 1;
        size = 1;
    }
    total_size = nmemb * size;
    if (total_size / nmemb != size)
        return (NULL);

    ptr = malloc(total_size);
    if (!ptr)
        return (NULL);
    ft_bzero(ptr, total_size);
    return (ptr);
}
