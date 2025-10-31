#include "../include/lemin.h"

void init_ants(t_lemin* lemin)
{
	if (!lemin || lemin->nb_ants <= 0)
		return;

	lemin->ants = malloc(sizeof(t_ant*) * lemin->nb_ants);
	if (!lemin->ants)
		return;

	Color colors[10] = { RED,  ORANGE, BLUE,   GREEN,	PURPLE,
						 GOLD, LIME,   VIOLET, SKYBLUE, PINK };

	for (int i = 0; i < lemin->nb_ants; i++)
	{
		t_ant* ant = malloc(sizeof(t_ant));
		if (!ant)
			continue;
		ant->id = i + 1;
		ant->current_room_id = lemin->start_id;
		ant->path = NULL;
		ant->path_position = 0;
		ant->color = colors[i % 10];
		lemin->ants[i] = ant;
	}
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		ft_printf("Usage: %s\n", argv[0]);
		ft_printf("Pipe lem-in output to this visualizer:\n");
		ft_printf("./lem-in < map | ./visu\n");
		return (1);
	}

	t_lemin* lemin = malloc(sizeof(t_lemin));
	if (!lemin)
	{
		ft_printf("ERROR: Memory allocation failed\n");
		return (1);
	}
	lemin->nb_ants = 0;
	lemin->nb_rooms = 0;
	lemin->rooms_by_id = NULL;
	lemin->start_id = -1;
	lemin->end_id = -1;
	lemin->moves_steps = NULL;
	lemin->ants = NULL;
	lemin->nb_ants_who_left_start = 0;
	lemin->capacity = NULL;
	lemin->used = NULL;

	int parsing_state = PARSING_ANTS;
	enum RoomType next_room_type = ROOM_TYPE_UNKNOWN;
	char* line = get_next_line_wnl(0);
	while (line)
	{
		if (*line == '\0')
		{
			free(line);
			break;
		}
		if (is_comment(line))
		{
			free(line);
			line = get_next_line_wnl(0);
			continue;
		}
		if (parsing_state == PARSING_ANTS)
			parsing_state = process_parsing_ants(lemin, line);
		else if (parsing_state == PARSING_ROOMS)
			parsing_state = parsing_rooms(lemin, line, &next_room_type);
		if (parsing_state == PARSING_LINKS)
		{
			char** link_rooms = parse_link(line);
			if (!link_rooms)
				ft_error("ERROR: Invalid link format in map.",
						 lemin,
						 __FILE__,
						 __LINE__);
			int id1 = get_room_id(lemin, link_rooms[0]);
			int id2 = get_room_id(lemin, link_rooms[1]);
			free_split(link_rooms);
			if (id1 == -1 || id2 == -1)
				ft_error("ERROR: Link references unknown room.",
						 lemin,
						 __FILE__,
						 __LINE__);
			integrate_link_rooms(lemin, id1, id2);
			integrate_link_rooms(lemin, id2, id1);
		}
		free(line);
		line = get_next_line_wnl(0);
	}

	if (lemin->start_id == -1 || lemin->end_id == -1)
	{
		ft_free_lemin(lemin);
		ft_printf("ERROR: Map must have start and end rooms\n");
		gnl_free_all_streams();
		return (1);
	}

	reset_room_states(lemin);

	init_ants(lemin);

	lemin->moves_steps = parse_moves(lemin, 0);

	run_visualization(lemin);

	if (lemin)
		ft_free_lemin(lemin);
	gnl_free_all_streams();
	return (0);
}
