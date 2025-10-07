#include "../../includes/lemin.h"

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
		ft_error("ERROR: Malloc failed for link ID.", NULL, __FILE__, __LINE__);
	*link_id = to_id;
	ft_lstadd_back(&from->links, ft_lstnew(link_id));
}

t_lemin* parsing(const char* input_file)
{
	if (!input_file)
		return (generate_test());

	// TODO: Implement actual parsing logic here
	return (NULL);
}
