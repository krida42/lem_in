#include "../../includes/lemin.h"

void free_move(t_move* move)
{
	if (!move)
		return;

	if (move->to_room_name)
	{
		free(move->to_room_name);
		move->to_room_name = NULL;
	}
	free(move);
}

void free_moves(t_list* moves)
{
	while (moves)
	{
		t_list* temp = moves;
		moves = moves->next;

		if (temp->content)
			free_move((t_move*)temp->content);
		free(temp);
	}
}

void init_ants(t_lemin* lemin, t_path** paths, int nb_paths)
{
	if (!lemin || !paths || nb_paths <= 0)
		return;

	lemin->ants = malloc(sizeof(t_ant*) * lemin->nb_ants);
	if (!lemin->ants)
		ft_error(
		  "ERROR: Malloc failed for ants array.", lemin, __FILE__, __LINE__);

	for (int i = 0; i < lemin->nb_ants; i++)
	{
		lemin->ants[i] = malloc(sizeof(t_ant));
		if (!lemin->ants[i])
			ft_error(
			  "ERROR: Malloc failed for ant.", lemin, __FILE__, __LINE__);

		lemin->ants[i]->id = i + 1; // 1 a n
		lemin->ants[i]->current_room_id = lemin->start_id;
		lemin->ants[i]->path_position = 0;

		// TODO: Optimiser la repartition des fourmis sur plusieurs paths
		lemin->ants[i]->path = paths[0];
	}
}

t_move* create_move(int ant_id,
					int from_room_id,
					int to_room_id,
					char* to_room_name)
{
	t_move* move = malloc(sizeof(t_move));
	if (!move)
		return (NULL);

	move->ant_id = ant_id;
	move->from_room_id = from_room_id;
	move->to_room_id = to_room_id;
	move->to_room_name = ft_strdup(to_room_name);

	return (move);
}

bool all_ants_finished(t_lemin* lemin)
{
	if (!lemin || !lemin->ants)
		return (true);

	for (int i = 0; i < lemin->nb_ants; i++)
	{
		if (lemin->ants[i]->current_room_id != lemin->end_id)
			return (false);
	}
	return (true);
}

bool is_room_occupied(t_lemin* lemin, int room_id, int exclude_ant_id)
{
	if (room_id == lemin->start_id || room_id == lemin->end_id)
		return (false);

	for (int i = 0; i < lemin->nb_ants; i++)
	{
		if (lemin->ants[i]->id != exclude_ant_id &&
			lemin->ants[i]->current_room_id == room_id)
			return (true);
	}
	return (false);
}

t_list* move_ants_one_turn(t_lemin* lemin)
{
	t_list* moves = NULL;

	if (!lemin || !lemin->ants)
		return (NULL);

	for (int i = 0; i < lemin->nb_ants; i++)
	{
		t_ant* ant = lemin->ants[i];

		if (ant->current_room_id == lemin->end_id)
			continue;

		int next_position = ant->path_position + 1;
		int next_room_id = get_room_id_from_path(ant->path, next_position);

		if (next_room_id == -1)
			continue;

		if (!is_room_occupied(lemin, next_room_id, ant->id))
		{
			t_move* move = create_move(ant->id,
									   ant->current_room_id,
									   next_room_id,
									   lemin->rooms_by_id[next_room_id]->name);
			if (move)
			{
				ft_lstadd_back(&moves, ft_lstnew(move));

				ant->current_room_id = next_room_id;
				ant->path_position = next_position;
			}
		}
	}

	return (moves);
}

void print_moves(t_list* moves)
{
	if (!moves)
		return;

	t_list* current = moves;
	bool first = true;

	while (current)
	{
		t_move* move = (t_move*)current->content;

		if (!first)
			ft_printf(" ");

		ft_printf("L%d-%s", move->ant_id, move->to_room_name);

		first = false;
		current = current->next;
	}
	ft_printf("\n");
}

void simulate_ants(t_lemin* lemin)
{
	if (!lemin)
		return;

	t_path** paths = find_multiple_paths(lemin, 3);
	if (!paths || !paths[0])
	{
		ft_printf("ERROR\n");
		return;
	}

	int nb_paths = count_valid_paths(paths, 3);
	init_ants(lemin, paths, nb_paths);

	while (!all_ants_finished(lemin))
	{
		t_list* moves = move_ants_one_turn(lemin);

		if (moves)
		{
			print_moves(moves);
			free_moves(moves);
		}
	}

	if (paths)
	{
		for (int i = 0; paths[i]; i++)
			free_path(paths[i]);
		free(paths);
	}
}
