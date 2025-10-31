#include "lemin.h"

void optimize_ants(t_lemin* lemin, t_path** paths, int nb_paths)
{
	int best_k = 1;
	int min_turns = INT_MAX;
	for (int k = 1; k <= nb_paths; k++)
	{
		int sum_len = 0;
		for (int i = 0; i < k; i++)
		{
			sum_len += paths[i]->len;
		}
		int turns =
		  (lemin->nb_ants + sum_len - 1) / k; // ceil((nb_ants + sum_len) / k)
		if (turns < min_turns)
		{
			min_turns = turns;
			best_k = k;
		}
	}
	int* ant_counts = calloc(best_k, sizeof(int));
	if (!ant_counts)
		return;
	for (int ant = 0; ant < lemin->nb_ants; ant++)
	{
		int best_p = 0;
		int min_cost = paths[0]->len + ant_counts[0];
		for (int p = 1; p < best_k; p++)
		{
			int cost = paths[p]->len + ant_counts[p];
			if (cost < min_cost)
			{
				min_cost = cost;
				best_p = p;
			}
		}
		ant_counts[best_p]++;
	}
	Color path_colors[10] = { RED,	ORANGE, BLUE,	GREEN,	 PURPLE,
							  GOLD, LIME,	VIOLET, SKYBLUE, PINK };
	lemin->ants = malloc(sizeof(t_ant*) * lemin->nb_ants);
	if (!lemin->ants)
	{
		free(ant_counts);
		return;
	}
	int ant_index = 0;
	for (int p = 0; p < best_k; p++)
	{
		for (int i = 0; i < ant_counts[p]; i++)
		{
			t_ant* ant = malloc(sizeof(t_ant));
			if (!ant)
				continue;
			ant->id = ant_index + 1;
			ant->current_room_id = lemin->start_id;
			ant->path = paths[p];
			ant->path_position = 0;
			ant->color = path_colors[p % 10];
			lemin->ants[ant_index] = ant;
			ant_index++;
		}
	}
	free(ant_counts);
}

void simulate_ants(t_lemin* lemin)
{
	int* room_occupants = malloc(sizeof(int) * lemin->nb_rooms);
	if (!room_occupants)
		return;
	for (int i = 0; i < lemin->nb_rooms; i++)
		room_occupants[i] = -1;
	int ants_at_end = 0;
	while (ants_at_end < lemin->nb_ants)
	{
		t_list* moves_this_turn = NULL;
		t_ant* sorted_ants[lemin->nb_ants];
		for (int i = 0; i < lemin->nb_ants; i++)
			sorted_ants[i] = lemin->ants[i];
		for (int i = 0; i < lemin->nb_ants - 1; i++)
		{
			for (int j = 0; j < lemin->nb_ants - i - 1; j++)
			{
				if (sorted_ants[j]->path_position <
					sorted_ants[j + 1]->path_position)
				{
					t_ant* temp = sorted_ants[j];
					sorted_ants[j] = sorted_ants[j + 1];
					sorted_ants[j + 1] = temp;
				}
			}
		}
		for (int i = 0; i < lemin->nb_ants; i++)
		{
			t_ant* ant = sorted_ants[i];
			if (ant->current_room_id != lemin->start_id &&
				ant->current_room_id != lemin->end_id)
			{
				t_list* temp = ant->path->room_ids;
				for (int j = 0; j <= ant->path_position; j++)
					temp = temp->next;
				int next_room_id = *(int*)temp->content;
				if (next_room_id == lemin->end_id ||
					room_occupants[next_room_id] == -1)
				{
					room_occupants[ant->current_room_id] = -1;

					ant->current_room_id = next_room_id;
					ant->path_position++;
					if (next_room_id != lemin->end_id)
						room_occupants[next_room_id] = ant->id;
					else
						ants_at_end++;

					char* id_str = ft_itoa(ant->id);
					char* temp1 = ft_strjoin("L", id_str);
					free(id_str);
					char* temp2 = ft_strjoin(temp1, "-");
					free(temp1);
					char* move_str =
					  ft_strjoin(temp2, lemin->rooms_by_id[next_room_id]->name);
					free(temp2);
					t_list* node = ft_lstnew(move_str);
					if (!node)
						free(move_str);
					else
						ft_lstadd_back(&moves_this_turn, node);
				}
			}
		}
		int nb_paths = 0;
		t_path* used_paths[lemin->nb_ants];
		for (int i = 0; i < lemin->nb_ants; i++)
		{
			t_path* p = lemin->ants[i]->path;
			bool found = false;
			for (int j = 0; j < nb_paths; j++)
			{
				if (used_paths[j] == p)
				{
					found = true;
					break;
				}
			}
			if (!found)
				used_paths[nb_paths++] = p;
		}
		bool* path_has_launched = calloc(nb_paths, sizeof(bool));
		if (!path_has_launched)
		{
			free(room_occupants);
			return;
		}
		for (int i = 0; i < lemin->nb_ants; i++)
		{
			t_ant* ant = lemin->ants[i];
			if (ant->current_room_id == lemin->start_id)
			{
				int path_index = -1;
				for (int j = 0; j < nb_paths; j++)
				{
					if (used_paths[j] == ant->path)
					{
						path_index = j;
						break;
					}
				}
				if (path_index == -1 || path_has_launched[path_index])
					continue;

				t_list* temp = ant->path->room_ids;
				temp = temp->next;
				int first_step_id = *(int*)temp->content;

				if (room_occupants[first_step_id] == -1)
				{
					ant->current_room_id = first_step_id;
					ant->path_position = 1;
					if (first_step_id != lemin->end_id)
						room_occupants[first_step_id] = ant->id;
					else
						ants_at_end++;

					char* id_str = ft_itoa(ant->id);
					char* temp1 = ft_strjoin("L", id_str);
					free(id_str);
					char* temp2 = ft_strjoin(temp1, "-");
					free(temp1);
					char* move_str = ft_strjoin(
					  temp2, lemin->rooms_by_id[first_step_id]->name);
					free(temp2);
					t_list* node = ft_lstnew(move_str);
					if (!node)
						free(move_str);
					else
						ft_lstadd_back(&moves_this_turn, node);
					path_has_launched[path_index] = true;
				}
			}
		}
		free(path_has_launched);
		if (moves_this_turn)
		{
			t_list* current = moves_this_turn;
			while (current)
			{
				ft_putstr_fd((char*)current->content, 1);
				if (current->next)
					ft_putstr_fd(" ", 1);
				current = current->next;
			}
			ft_putstr_fd("\n", 1);
			ft_lstclear(&moves_this_turn, free);
		}
	}
	free(room_occupants);
}
