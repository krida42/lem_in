#include "lemin.h"

int main(void)
{
	t_lemin lemin;

	init_lemin(&lemin);
	parse_input(&lemin);

	free_lemin(&lemin);

	return (0);
}
