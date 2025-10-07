#ifndef LEMIN_H
#define LEMIN_H

#include "../libft/include/libft.h"
#include <limits.h>	 // INT_MAX
#include <stdbool.h> // true, false
#include <unistd.h>	 // write

#define PARSING_ANTS 0
#define PARSING_ROOMS 1
#define PARSING_LINKS 2

#define ERROR -1
#define SUCCESS 0

typedef struct s_room
{
	char* name;
	int id;
	// int x;
	// int y;

	t_list* links;

	bool visited;
	int parent_id;
} t_room;

typedef struct s_path
{
	t_list* room_ids;
	int len;
} t_path;

typedef struct s_lemin
{
	int nb_ants;
	int nb_rooms;

	t_room** rooms_by_id;

	int start_id; // ID de ##start
	int end_id;	  // ID de ##end
} t_lemin;

#endif
