#include "lemin.h"

void reset_room_states(t_lemin* lemin)
{
	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		lemin->rooms_by_id[i]->visited = false;
		lemin->rooms_by_id[i]->parent_id = -1;
	}
}

t_path* create_path()
{
	t_path* path = malloc(sizeof(t_path));
	if (!path)
		return NULL;
	path->room_ids = NULL;
	path->len = 0;
	return path;
}

void free_path(t_path* path)
{
	if (!path)
		return;
	if (path->room_ids)
	{
		ft_lstclear(&path->room_ids, free);
	}
	free(path);
}

t_path* bfs_find_path(t_lemin* lemin, int start_id, int end_id)
{
	reset_room_states(lemin);
	t_list* queue = NULL;
	int* start_ptr = malloc(sizeof(int));
	if (!start_ptr)
		return NULL;
	*start_ptr = start_id;
	ft_lstadd_back(&queue, ft_lstnew(start_ptr));
	lemin->rooms_by_id[start_id]->visited = true;
	lemin->rooms_by_id[start_id]->parent_id = -1;
	bool found = false;
	while (queue)
	{
		t_list* node = queue;
		int u = *(int*)node->content;
		queue = queue->next;
		ft_lstdelone(node, free);
		if (u == end_id)
		{
			found = true;
			break;
		}
		t_list* links = lemin->rooms_by_id[u]->links;
		t_list* current = links;
		while (current)
		{
			int v = *(int*)current->content;
			if (!lemin->rooms_by_id[v]->visited &&
				lemin->capacity[u][v] - lemin->used[u][v] > 0 &&
				(!lemin->rooms_by_id[v]->is_on_path || v == end_id))
			{
				lemin->rooms_by_id[v]->visited = true;
				lemin->rooms_by_id[v]->parent_id = u;
				int* v_ptr = malloc(sizeof(int));
				if (!v_ptr)
				{
					ft_lstclear(&queue, free);
					return NULL;
				}
				else
				{
					*v_ptr = v;
					ft_lstadd_back(&queue, ft_lstnew(v_ptr));
				}
			}
			current = current->next;
		}
	}
	ft_lstclear(&queue, free);
	if (!found)
		return NULL;

	t_path* path = create_path();
	if (!path)
		return NULL;

	t_list* ids = NULL;
	int current_id = end_id;
	while (current_id != -1)
	{
		int* id_ptr = malloc(sizeof(int));
		if (!id_ptr)
		{
			free_path(path);
			return NULL;
		}
		*id_ptr = current_id;
		ft_lstadd_front(&ids, ft_lstnew(id_ptr));
		current_id = lemin->rooms_by_id[current_id]->parent_id;
	}
	path->room_ids = ids;
	path->len = ft_lstsize(ids);
	return path;
}

t_path** find_multiple_paths(t_lemin* lemin, int max_paths)
{
	int array_size = max_paths + 1;
	t_path** paths = malloc(sizeof(t_path*) * array_size);
	if (!paths)
		return NULL;

	for (int i = 0; i < lemin->nb_rooms; i++)
		lemin->rooms_by_id[i]->is_on_path = false;

	int path_count = 0;
	while (path_count < max_paths)
	{
		t_path* path = bfs_find_path(lemin, lemin->start_id, lemin->end_id);
		if (!path)
			break;
		paths[path_count] = path;
		path_count++;

		t_list* current = path->room_ids;
		while (current)
		{
			int u = *(int*)current->content;
			if (u != lemin->start_id && u != lemin->end_id)
				lemin->rooms_by_id[u]->is_on_path = true;
			current = current->next;
		}

		current = path->room_ids;
		int prev = -1;
		while (current)
		{
			int u = *(int*)current->content;
			if (prev != -1)
			{
				lemin->used[prev][u]++;
				lemin->used[u][prev]--;
			}
			prev = u;
			current = current->next;
		}
	}
	paths[path_count] = NULL;
	return paths;
}
