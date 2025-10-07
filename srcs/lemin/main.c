#include "../../includes/lemin.h"

void display_lemin(t_lemin* lemin)
{
	ft_printf("Number of ants: %d\n", lemin->nb_ants);
	ft_printf("Number of rooms: %d\n", lemin->nb_rooms);
	ft_printf("Start room ID: %d\n", lemin->start_id);
	ft_printf("End room ID: %d\n", lemin->end_id);
	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		ft_printf("Room %d (%s) links to: ", room->id, room->name);
		t_list* link = room->links;
		while (link)
		{
			int* linked_id = (int*)link->content;
			ft_printf("%d ", *linked_id);
			link = link->next;
		}
		ft_printf("\n");
	}
}

int main(int argc, const char* argv[])
{
	if (argc > 2)
	{
		ft_printf("Usage: %s [input_file]\n", argv[0]);
		return (1);
	}

	const char* input_file = (argc == 2) ? argv[1] : NULL;

	t_lemin* lemin = parsing(input_file);
	if (!lemin)
		ft_error(
		  "ERROR: Failed to generate test graph.", NULL, __FILE__, __LINE__);

	ft_printf("%d\n", lemin->nb_ants);

	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		if (i == lemin->start_id)
			ft_printf("##start\n");
		if (i == lemin->end_id)
			ft_printf("##end\n");
		ft_printf("%s %d %d\n", room->name, i, i); // x=id, y=id
	}

	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		t_list* link = room->links;
		while (link)
		{
			int linked_id = *(int*)link->content;
			if (i < linked_id)
				ft_printf(
				  "%s-%s\n", room->name, lemin->rooms_by_id[linked_id]->name);
			link = link->next;
		}
	}

	ft_printf("\n");

	simulate_ants(lemin);

	if (lemin)
	{
		ft_free_lemin(lemin);
		lemin = NULL;
	}
	return (0);
}
