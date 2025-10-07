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

// utils.c
void ft_error(char* msg, t_lemin* lemin, const char* file, int line);
void ft_free_lemin(t_lemin* lemin);

// parsing.c
void init_lemin(t_lemin* lemin);
t_room* create_room(int id, const char* name);
void add_directed_link(t_room* from, int to_id);
t_lemin* parsing(const char* input_file);

// test_generator.c
t_lemin* generate_test(void);

#endif
