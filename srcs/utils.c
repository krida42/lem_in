#include "../include/lemin.h"

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
	move->time_interpolation = 0.0f;

	return (move);
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

	free(lemin);
}

void ft_error(char* msg, t_lemin* lemin, const char* file, int line)
{
	(void)msg;
	(void)file;
	(void)line;

	ft_putendl_fd("ERROR", 1);

	if (lemin)
		ft_free_lemin(lemin);
	exit(EXIT_FAILURE);
}
