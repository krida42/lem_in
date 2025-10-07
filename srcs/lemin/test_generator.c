#include "../../includes/lemin.h"

t_lemin* generate_test(void)
{
	t_lemin* lemin;
	const char* room_names[] = { "Start", "A", "B", "C",  "D",
								 "E",	  "F", "G", "End" };
	int i;

	// lemin = malloc(sizeof(t_lemin));
	lemin = NULL;

	if (!lemin)
		ft_error("ERROR: Malloc failed for lemin structure.",
				 NULL,
				 __FILE__,
				 __LINE__);
	init_lemin(lemin);
	lemin->nb_ants = 10;
	lemin->nb_rooms = 9;
	lemin->start_id = 0;
	lemin->end_id = 8;

	lemin->rooms_by_id = malloc(sizeof(t_room*) * lemin->nb_rooms);
	if (!lemin->rooms_by_id)
		ft_error("ERROR: Malloc failed for rooms_by_id array.",
				 lemin,
				 __FILE__,
				 __LINE__);

	i = 0;
	while (i < lemin->nb_rooms)
	{
		lemin->rooms_by_id[i] = create_room(i, room_names[i]);
		if (!lemin->rooms_by_id[i])
			ft_error(
			  "ERROR: Failed to create a room.", lemin, __FILE__, __LINE__);
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
