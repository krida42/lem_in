#include "../include/lemin.h"

int distribution_cost(int* ants_per_path, t_path** paths, int nb_paths)
{
	int max_turns = 0;

	for (int i = 0; i < nb_paths; i++)
	{
		if (!paths[i])
			continue;

		int turns = ants_per_path[i] + paths[i]->len - 1;

		if (turns > max_turns)
			max_turns = turns;
	}

	return (max_turns);
}

void optimize_ants(t_lemin* lemin, t_path** paths, int nb_paths)
{
	if (!lemin || !paths || nb_paths <= 0)
		return;

	lemin->ants = malloc(sizeof(t_ant*) * lemin->nb_ants);
	if (!lemin->ants)
		ft_error(
		  "ERROR: Malloc failed for ants array.", lemin, __FILE__, __LINE__);

	int* ants_per_paths = malloc(sizeof(int) * nb_paths);
	if (!ants_per_paths)
		ft_error(
		  "ERROR: Malloc failed for ants_per_path.", lemin, __FILE__, __LINE__);

	for (int i = 0; i < nb_paths; i++)
		ants_per_paths[i] = 0;

	for (int ant = 0; ant < lemin->nb_ants; ant++)
	{
		int best_path = 0;
		int min_turns = INT_MAX;

		for (int i = 0; i < nb_paths; i++)
		{
			if (!paths[i])
				continue;

			ants_per_paths[i]++;
			int turns = distribution_cost(ants_per_paths, paths, nb_paths);

			if (turns < min_turns)
			{
				min_turns = turns;
				best_path = i;
			}

			ants_per_paths[i]--;
		}

		ants_per_paths[best_path]++;
	}

	Color path_colors[] = { ORANGE, RED,   GOLD, PINK,	  BLUE,
							PURPLE, GREEN, LIME, SKYBLUE, MAROON };
	int num_colors = sizeof(path_colors) / sizeof(path_colors[0]);

	int ant_index = 0;
	for (int path_idx = 0; path_idx < nb_paths; path_idx++)
	{
		for (int i = 0; i < ants_per_paths[path_idx]; i++)
		{
			lemin->ants[ant_index] = malloc(sizeof(t_ant));
			if (!lemin->ants[ant_index])
				ft_error(
				  "ERROR: Malloc failed for ant.", lemin, __FILE__, __LINE__);

			lemin->ants[ant_index]->id = ant_index + 1;
			lemin->ants[ant_index]->current_room_id = lemin->start_id;
			lemin->ants[ant_index]->path_position = 0;
			lemin->ants[ant_index]->path = paths[path_idx];
			lemin->ants[ant_index]->color = path_colors[path_idx % num_colors];

			ant_index++;
		}
	}

	free(ants_per_paths);
}

bool finish_ants(t_lemin* lemin)
{
	if (!lemin || !lemin->ants)
		return (true);

	for (int i = 0; i < lemin->nb_ants; i++)
		if (lemin->ants[i]->current_room_id != lemin->end_id)
			return (false);
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

	bool* target_occupied = calloc(lemin->nb_rooms, sizeof(bool));
	if (!target_occupied)
		ft_error("ERROR: Malloc failed for target_occupied.",
				 lemin,
				 __FILE__,
				 __LINE__);

	for (int i = lemin->nb_ants - 1; i >= 0; i--)
	{
		t_ant* ant = lemin->ants[i];

		if (ant->current_room_id == lemin->end_id)
			continue;

		int next_position = ant->path_position + 1;
		int next_room_id = get_room_id_from_path(ant->path, next_position);

		if (next_room_id == -1)
			continue;

		if (!target_occupied[next_room_id] &&
			!is_room_occupied(lemin, next_room_id, ant->id))
		{
			t_move* move = create_move(ant->id,
									   ant->current_room_id,
									   next_room_id,
									   lemin->rooms_by_id[next_room_id]->name);
			if (move)
			{
				ft_lstadd_back(&moves, ft_lstnew(move));
				target_occupied[next_room_id] = true;
			}
		}
	}

	free(target_occupied);

	t_list* move_node = moves;
	while (move_node)
	{
		t_move* move = (t_move*)move_node->content;
		if (move)
		{
			t_ant* ant = lemin->ants[move->ant_id - 1];
			ant->current_room_id = move->to_room_id;
			ant->path_position = ant->path_position + 1;
		}
		move_node = move_node->next;
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

	t_path** paths = edmonds_karp(lemin);
	if (!paths || !paths[0])
	{
		ft_error(
		  "ERROR: No paths found for simulation.", lemin, __FILE__, __LINE__);
		return;
	}

	int nb_paths = count_valid_paths(paths, lemin->nb_rooms);

	optimize_ants(lemin, paths, nb_paths);

	while (!finish_ants(lemin))
	{
		t_list* moves = move_ants_one_turn(lemin);

		if (moves)
		{
			print_moves(moves);
			t_list* new_node = ft_lstnew(moves);

			ft_lstadd_back(&lemin->moves_steps, new_node);
		}
	}

	if (paths)
	{
		for (int i = 0; paths[i]; i++)
			free_path(paths[i]);
		free(paths);
	}
}
