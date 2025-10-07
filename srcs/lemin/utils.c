#include "../../includes/lemin.h"

void ft_free_lemin(t_lemin* lemin)
{
	if (!lemin)
		return;

	if (lemin->ants)
	{
		for (int i = 0; i < lemin->nb_ants; i++)
		{
			if (lemin->ants[i])
			{
				free(lemin->ants[i]);
				lemin->ants[i] = NULL;
			}
		}
		free(lemin->ants);
		lemin->ants = NULL;
	}

	if (lemin->rooms_by_id)
	{
		for (int i = 0; i < lemin->nb_rooms; i++)
		{
			t_room* room = lemin->rooms_by_id[i];
			if (room)
			{
				if (room->name)
				{
					free(room->name);
					room->name = NULL;
				}

				t_list* link = room->links;
				while (link)
				{
					t_list* temp = link;
					link = link->next;
					if (temp->content)
					{
						free(temp->content);
						temp->content = NULL;
					}
					free(temp);
				}
				room->links = NULL;
				free(room);
			}
		}
		free(lemin->rooms_by_id);
		lemin->rooms_by_id = NULL;
	}

	free(lemin);
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
