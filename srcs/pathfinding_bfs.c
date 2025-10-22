#include "../include/lemin.h"

t_path* create_path(void)
{
	t_path* path = malloc(sizeof(t_path));
	if (!path)
		return (NULL);
	path->room_ids = NULL;
	path->len = 0;
	return (path);
}

void add_room_to_path(t_path* path, int room_id)
{
	int* id = malloc(sizeof(int));
	if (!id)
		return;
	*id = room_id;
	ft_lstadd_back(&path->room_ids, ft_lstnew(id));
	path->len++;
}

void free_path(t_path* path)
{
	if (!path)
		return;

	t_list* current = path->room_ids;
	while (current)
	{
		t_list* temp = current;
		current = current->next;
		if (temp->content)
			free(temp->content);
		free(temp);
	}
	free(path);
}

void reset_room_states(t_lemin* lemin)
{
	if (!lemin || !lemin->rooms_by_id)
		return;

	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		if (lemin->rooms_by_id[i])
		{
			lemin->rooms_by_id[i]->visited = false;
			lemin->rooms_by_id[i]->parent_id = -1;
		}
	}
}

t_path* reconstruct_path(t_lemin* lemin, int start_id, int end_id)
{
	t_path* path = create_path();
	if (!path)
		return (NULL);

	int current_id = end_id;
	t_list* temp_path = NULL;

	while (current_id != -1)
	{
		int* id = malloc(sizeof(int));
		if (!id)
		{
			while (temp_path)
			{
				t_list* temp = temp_path;
				temp_path = temp_path->next;
				free(temp->content);
				free(temp);
			}
			free_path(path);
			return (NULL);
		}
		*id = current_id;
		ft_lstadd_front(&temp_path, ft_lstnew(id));

		if (current_id == start_id)
			break;
		current_id = lemin->rooms_by_id[current_id]->parent_id;
	}

	while (temp_path)
	{
		t_list* temp = temp_path;
		temp_path = temp_path->next;
		int* room_id = (int*)temp->content;
		add_room_to_path(path, *room_id);
		free(room_id);
		free(temp);
	}

	return (path);
}

t_path* bfs_find_path(t_lemin* lemin, int start_id, int end_id)
{
	if (!lemin || !lemin->rooms_by_id || start_id < 0 || end_id < 0 ||
		start_id >= lemin->nb_rooms || end_id >= lemin->nb_rooms)
		return (NULL);

	reset_room_states(lemin);

	t_list* queue = NULL;

	lemin->rooms_by_id[start_id]->visited = true;
	lemin->rooms_by_id[start_id]->parent_id = -1;

	int* start_ptr = malloc(sizeof(int));
	if (!start_ptr)
		return (NULL);
	*start_ptr = start_id;
	ft_lstadd_back(&queue, ft_lstnew(start_ptr));

	while (queue)
	{
		t_list* current_node = queue;
		int current_id = *(int*)current_node->content;
		queue = queue->next;
		free(current_node->content);
		free(current_node);

		if (current_id == end_id)
		{
			while (queue)
			{
				t_list* temp = queue;
				queue = queue->next;
				free(temp->content);
				free(temp);
			}
			return (reconstruct_path(lemin, start_id, end_id));
		}

		t_room* current_room = lemin->rooms_by_id[current_id];
		t_list* link = current_room->links;

		while (link)
		{
			int neighbor_id = *(int*)link->content;

			if (!lemin->rooms_by_id[neighbor_id]->visited)
			{
				lemin->rooms_by_id[neighbor_id]->visited = true;
				lemin->rooms_by_id[neighbor_id]->parent_id = current_id;

				int* neighbor_ptr = malloc(sizeof(int));
				if (!neighbor_ptr)
				{
					while (queue)
					{
						t_list* temp = queue;
						queue = queue->next;
						free(temp->content);
						free(temp);
					}
					return (NULL);
				}
				*neighbor_ptr = neighbor_id;
				ft_lstadd_back(&queue, ft_lstnew(neighbor_ptr));
			}
			link = link->next;
		}
	}

	return (NULL);
}

int get_room_id_from_path(t_path* path, int position)
{
	if (!path || !path->room_ids || position < 0)
		return (-1);

	t_list* current = path->room_ids;
	int i = 0;

	while (current && i < position)
	{
		current = current->next;
		i++;
	}

	if (current)
		return (*(int*)current->content);
	return (-1);
}

int count_valid_paths(t_path** paths, int max_paths)
{
	int count = 0;

	for (int i = 0; i < max_paths && paths[i]; i++)
		count++;

	return (count);
}

bool is_room_in_path(t_path* path, int room_id, bool exclude_endpoints)
{
	if (!path || !path->room_ids)
		return (false);

	t_list* current = path->room_ids;
	int position = 0;

	while (current)
	{
		int path_room_id = *(int*)current->content;

		if (exclude_endpoints && (position == 0 || current->next == NULL))
		{
			current = current->next;
			position++;
			continue;
		}

		if (path_room_id == room_id)
			return (true);

		current = current->next;
		position++;
	}
	return (false);
}

bool paths_inter_rooms(t_path* path1, t_path* path2)
{
	if (!path1 || !path2 || !path1->room_ids || !path2->room_ids)
		return (false);

	t_list* current = path1->room_ids->next; // skip start

	while (current && current->next) // skip end
	{
		int room_id = *(int*)current->content;

		if (is_room_in_path(path2, room_id, true))
			return (true);

		current = current->next;
	}

	return (false);
}

t_path* bfs_find_otherpath(t_lemin* lemin,
						   int start_id,
						   int end_id,
						   t_path** existing_paths,
						   int nb_existing)
{
	if (!lemin || !lemin->rooms_by_id || start_id < 0 || end_id < 0 ||
		start_id >= lemin->nb_rooms || end_id >= lemin->nb_rooms)
		return (NULL);

	reset_room_states(lemin);

	for (int i = 0; i < nb_existing; i++)
	{
		if (!existing_paths[i])
			continue;

		t_list* current = existing_paths[i]->room_ids;
		int position = 0;

		while (current)
		{
			int room_id = *(int*)current->content;

			if (room_id != start_id && room_id != end_id)
				lemin->rooms_by_id[room_id]->visited = true;

			current = current->next;
			position++;
		}
	}

	t_list* queue = NULL;

	lemin->rooms_by_id[start_id]->visited = true;
	lemin->rooms_by_id[start_id]->parent_id = -1;

	int* start_ptr = malloc(sizeof(int));
	if (!start_ptr)
		return (NULL);

	*start_ptr = start_id;
	ft_lstadd_back(&queue, ft_lstnew(start_ptr));

	while (queue)
	{
		t_list* current_node = queue;
		int current_id = *(int*)current_node->content;
		queue = queue->next;
		free(current_node->content);
		free(current_node);

		if (current_id == end_id)
		{
			while (queue)
			{
				t_list* temp = queue;
				queue = queue->next;
				free(temp->content);
				free(temp);
			}
			return (reconstruct_path(lemin, start_id, end_id));
		}

		t_room* current_room = lemin->rooms_by_id[current_id];
		t_list* link = current_room->links;

		while (link)
		{
			int neighbor_id = *(int*)link->content;

			if (!lemin->rooms_by_id[neighbor_id]->visited)
			{
				lemin->rooms_by_id[neighbor_id]->visited = true;
				lemin->rooms_by_id[neighbor_id]->parent_id = current_id;

				int* neighbor_ptr = malloc(sizeof(int));
				if (!neighbor_ptr)
				{
					while (queue)
					{
						t_list* temp = queue;
						queue = queue->next;
						free(temp->content);
						free(temp);
					}
					return (NULL);
				}
				*neighbor_ptr = neighbor_id;
				ft_lstadd_back(&queue, ft_lstnew(neighbor_ptr));
			}
			link = link->next;
		}
	}

	return (NULL);
}

t_path** find_multiple_paths(t_lemin* lemin, int max_paths)
{
	if (!lemin || max_paths <= 0)
		return (NULL);

	t_path** paths = malloc(sizeof(t_path*) * (max_paths + 1));
	if (!paths)
		return (NULL);

	for (int i = 0; i <= max_paths; i++)
		paths[i] = NULL;

	paths[0] = bfs_find_path(lemin, lemin->start_id, lemin->end_id);

	if (!paths[0])
		return (paths);

	int found_paths = 1;
	for (int i = 1; i < max_paths; i++)
	{
		t_path* new_path = bfs_find_otherpath(
		  lemin, lemin->start_id, lemin->end_id, paths, found_paths);

		if (!new_path)
			break;

		paths[found_paths] = new_path;
		found_paths++;
	}

	return (paths);
}
