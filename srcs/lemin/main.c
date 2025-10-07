#include "../../includes/lemin.h"

void ft_error(const char* msg, t_lemin* lemin)
{
	if (msg)
		ft_printf("%s\n", msg);

	if (lemin)
	{
		if (lemin->rooms_by_id)
		{
			for (int i = 0; i < lemin->nb_rooms; i++)
			{
				if (lemin->rooms_by_id[i])
				{
					if (lemin->rooms_by_id[i]->name)
						free(lemin->rooms_by_id[i]->name);
					if (lemin->rooms_by_id[i]->links)
						ft_lstclear(&lemin->rooms_by_id[i]->links, free);
					free(lemin->rooms_by_id[i]);
				}
			}
			free(lemin->rooms_by_id);
		}
		free(lemin);
	}
	exit(EXIT_FAILURE);
}

void init_lemin(t_lemin* lemin)
{
	lemin->nb_ants = 0;
	lemin->nb_rooms = 0;
	lemin->rooms_by_id = NULL;
	lemin->start_id = -1;
	lemin->end_id = -1;
}

t_room* create_room(int id, const char* name)
{
	t_room* room;

	room = malloc(sizeof(t_room));
	if (!room)
		return (NULL);
	room->id = id;
	room->name = ft_strdup(name);
	if (!room->name)
	{
		free(room);
		return (NULL);
	}
	room->links = NULL;
	return (room);
}

void add_directed_link(t_room* from, int to_id)
{
	int* link_id = malloc(sizeof(int));
	if (!link_id)
		ft_error("ERROR: Malloc failed for link ID.", NULL);
	*link_id = to_id;
	ft_lstadd_back(&from->links, ft_lstnew(link_id));
}

t_lemin* generate_test_graph(void)
{
	t_lemin* lemin;
	const char* room_names[] = { "Start", "A", "B", "C",  "D",
								 "E",	  "F", "G", "End" };
	int i;

	lemin = malloc(sizeof(t_lemin));

	if (!lemin)
		ft_error("ERROR: Malloc failed for lemin structure.", NULL);
	init_lemin(lemin);
	lemin->nb_ants = 10;
	lemin->nb_rooms = 9;
	lemin->start_id = 0;
	lemin->end_id = 8;

	lemin->rooms_by_id = malloc(sizeof(t_room*) * lemin->nb_rooms);
	if (!lemin->rooms_by_id)
		ft_error("ERROR: Malloc failed for rooms_by_id array.", lemin);

	i = 0;
	while (i < lemin->nb_rooms)
	{
		lemin->rooms_by_id[i] = create_room(i, room_names[i]);
		if (!lemin->rooms_by_id[i])
			ft_error("ERROR: Failed to create a room.", lemin);
		i++;
	}

	add_directed_link(lemin->rooms_by_id[0], 1); // Start -> A
	add_directed_link(lemin->rooms_by_id[0], 2); // Start -> B
	add_directed_link(lemin->rooms_by_id[1], 3); // A -> C
	add_directed_link(lemin->rooms_by_id[1], 5); // A -> E
	add_directed_link(lemin->rooms_by_id[2], 4); // B -> D
	add_directed_link(lemin->rooms_by_id[3], 4); // C -> D
	add_directed_link(lemin->rooms_by_id[4], 7); // D -> G
	add_directed_link(lemin->rooms_by_id[4], 8); // D -> End
	add_directed_link(lemin->rooms_by_id[5], 4); // E -> D
	add_directed_link(lemin->rooms_by_id[5], 6); // E -> F
	add_directed_link(lemin->rooms_by_id[6], 7); // F -> G
	add_directed_link(lemin->rooms_by_id[7], 8); // G -> End

	return (lemin);
}

int main(void)
{
	t_lemin* lemin = generate_test_graph();
	if (!lemin)
		ft_error("ERROR: Failed to generate test graph.", NULL);

	// Simple output to verify the graph structure
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
	return (0);
}
