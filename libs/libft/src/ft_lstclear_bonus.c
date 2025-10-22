#include "../include/libft.h"
void ft_lstclear(t_list **lst, void (*del)(void *))
{
    t_list *current;
    t_list *next_node;

    if (!lst || !*lst || !del)
        return;
    current = *lst;
    while (current != NULL)
    {
        next_node = current->next;
        ft_lstdelone(current, del);
        current = next_node;
    }
    *lst = NULL;
}
