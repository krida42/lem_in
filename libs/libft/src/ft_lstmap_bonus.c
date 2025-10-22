#include "../include/libft.h"
t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
    t_list *new_list_head;
    t_list *new_node;
    t_list *current;
    void *new_content;

    if (!lst || !f || !del)
        return (NULL);

    new_list_head = NULL;
    current = lst;
    while (current != NULL)
    {
        new_content = f(current->content);

        new_node = ft_lstnew(new_content);
        if (!new_node)
        {
            del(new_content);
            ft_lstclear(&new_list_head, del);
            return (NULL);
        }
        ft_lstadd_back(&new_list_head, new_node);

        current = current->next;
    }
    return (new_list_head);
}
