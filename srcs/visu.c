#include "../include/lemin.h"

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
		return (1);
	}

	reset_room_states(lemin);

	t_path** paths = find_multiple_paths(lemin, lemin->nb_rooms);
	if (!paths || !paths[0])
	{
		ft_free_lemin(lemin);
		ft_printf("ERROR: No path found between start and end\n");
		return (1);
	}
	int nb_paths = count_valid_paths(paths, lemin->nb_rooms);
	optimize_ants(lemin, paths, nb_paths);

	lemin->moves_steps = parse_moves(lemin, 0);

	run_visualization(lemin);

	if (paths)
	{
		for (int i = 0; paths[i]; i++)
			free_path(paths[i]);
		free(paths);
	}

	if (lemin)
		ft_free_lemin(lemin);
	return (0);
}
