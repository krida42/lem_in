#include "../include/lemin.h"
#include <unistd.h>

int ft_strcmp(const char* s1, const char* s2)
{
	return ft_strncmp(s1, s2, INT_MAX);
}

char* get_next_line_wnl(int fd)
{
	char* str = get_next_line(fd);

	if (!str)
		return NULL;
	char* newline_pos = ft_strchr(str, '\n');
	if (newline_pos)
		*newline_pos = '\0';
	return str;
}

void free_split(char** split_array)
{
	int i;

	i = 0;
	if (!split_array)
		return;
	while (split_array[i])
	{
		free(split_array[i]);
		i++;
	}
	free(split_array);
}

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
	if (!move->to_room_name)
	{
		free(move);
		return (NULL);
	}
	move->time_interpolation = 0.0f;

	return (move);
}

t_list* parse_moves(t_lemin* lemin, int fd)
{
	char* line = get_next_line_wnl(fd);
	t_list* moves_steps = NULL;

	int* ant_positions = malloc(sizeof(int) * lemin->nb_ants);
	if (!ant_positions)
		return (NULL);
	for (int i = 0; i < lemin->nb_ants; i++)
		ant_positions[i] = lemin->start_id;

	while (line)
	{
		if (*line == '\0')
		{
			free(line);
			line = get_next_line_wnl(fd);
			continue;
		}

		char** move_parts = ft_split(line, ' ');
		if (!move_parts)
		{
			free(line);
			continue;
		}

		t_list* turn_moves = NULL;
		int i = 0;
		while (move_parts[i])
		{
			if (ft_strncmp(move_parts[i], "L", 1) == 0)
			{
				char* dash_pos = ft_strchr(move_parts[i], '-');
				if (dash_pos)
				{
					*dash_pos = '\0';
					int ant_id = ft_atoi(move_parts[i] + 1);
					char* room_name = dash_pos + 1;
					int room_id = get_room_id(lemin, room_name);

					if (room_id != -1 && ant_id > 0 && ant_id <= lemin->nb_ants)
					{
						int from_id = ant_positions[ant_id - 1];
						t_move* move =
						  create_move(ant_id, from_id, room_id, room_name);
						if (move)
						{
							t_list* new_node = ft_lstnew(move);
							if (!new_node)
								free_move(move);
							else
								ft_lstadd_back(&turn_moves, new_node);
						}
					}
				}
			}
			i++;
		}

		t_list* current_move = turn_moves;
		while (current_move)
		{
			t_move* move = (t_move*)current_move->content;
			ant_positions[move->ant_id - 1] = move->to_room_id;
			current_move = current_move->next;
		}

		free_split(move_parts);
		if (turn_moves)
			ft_lstadd_back(&moves_steps, ft_lstnew(turn_moves));
		free(line);
		line = get_next_line_wnl(fd);
	}

	free(ant_positions);
	return (moves_steps);
}

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

	if (lemin->moves_steps)
	{
		t_list* current_step = lemin->moves_steps;
		while (current_step)
		{
			t_list* moves = (t_list*)current_step->content;
			free_moves(moves);

			t_list* temp_step = current_step;
			current_step = current_step->next;
			free(temp_step);
		}
		lemin->moves_steps = NULL;
	}

	if (lemin->capacity)
	{
		for (int i = 0; i < lemin->nb_rooms; i++)
			if (lemin->capacity[i])
				free(lemin->capacity[i]);
		free(lemin->capacity);
		lemin->capacity = NULL;
	}

	if (lemin->used)
	{
		for (int i = 0; i < lemin->nb_rooms; i++)
			if (lemin->used[i])
				free(lemin->used[i]);
		free(lemin->used);
		lemin->used = NULL;
	}

	free(lemin);
}

void ft_error(char* msg, t_lemin* lemin, const char* file, int line)
{
	// (void)msg;
	// (void)file;
	// (void)line;

	ft_printf("%s (at %s:%d)\n", msg, file, line);

	if (lemin)
		ft_free_lemin(lemin);
	gnl_free_all_streams();
	exit(EXIT_FAILURE);
}
