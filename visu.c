#include "include/lemin.h"

int main(void)
{
	t_lemin* lemin = parse(0);

	t_path** paths = find_multiple_paths(lemin, lemin->nb_rooms);
	if (!paths || !paths[0])
	{
		ft_free_lemin(lemin);
		ft_printf("ERROR: No path found between start and end\n");
		return (1);
	}
	int nb_paths = count_valid_paths(paths, lemin->nb_rooms);
	optimize_ants(lemin, paths, nb_paths);

	run_visualization(lemin);

	if (paths)
	{
		for (int i = 0; paths[i]; i++)
			free_path(paths[i]);
		free(paths);
	}

	if (lemin)
	{
		ft_free_lemin(lemin);
		lemin = NULL;
	}
	return (0);
}
