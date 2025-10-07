#include "../../includes/lemin.h"

void ft_free_lemin(t_lemin* lemin)
{
	if (lemin)
	{
		if (lemin->rooms_by_id)
		{
			for (int i = 0; i < lemin->nb_rooms; i++)
			{
				t_room* room = lemin->rooms_by_id[i];
				if (room)
				{
					free(room->name);
					t_list* link = room->links;
					while (link)
					{
						free(link->content);
						t_list* temp = link;
						link = link->next;
						free(temp);
					}
					free(room);
				}
			}
			free(lemin->rooms_by_id);
		}
		free(lemin);
	}
}

void ft_error(char* msg, t_lemin* lemin, const char* file, int line)
{
	ft_printf("ERROR [%s:%d]: %s\n", file, line, msg);

	if (msg)
		ft_printf("%s\n", msg);

	if (lemin)
	{
		ft_printf("Freeing lemin structure...\n");
		ft_free_lemin(lemin);
	}
	exit(EXIT_FAILURE);
}
