#ifndef LEMIN_H
#define LEMIN_H

#include "libft.h"
#include "get_next_line.h"
#include <limits.h>	 // INT_MAX
#include <stdbool.h> // true, false
#include <unistd.h>	 // write
#include <stdio.h> // printf (pour les test pendant le dev) - -  A RETIRER



#define ERROR -1
#define SUCCESS 0


enum RoomType {
	ROOM_TYPE_UNKNOWN,
	ROOM_START,
	ROOM_END
};

enum ParsingState {
	PARSING_ANTS,
	PARSING_ROOMS,
	PARSING_LINKS
};

typedef struct s_room
{
	char* name;
	int id;
	int x;
	int y;

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


t_lemin *parse(int fd);
void ft_error(const char* msg, t_lemin* lemin);
void lemin_print_map_pretty(const t_lemin *lem);
void free_everything_lemin(t_lemin *lemin);

#endif
