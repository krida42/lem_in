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

typedef struct s_ant
{
	int id;
	int current_room_id;
	t_path* path;
	int path_position; // 0 = start
} t_ant;

typedef struct s_move
{
	int ant_id;
	int from_room_id;
	int to_room_id;
	char* to_room_name;
} t_move;

typedef struct s_lemin
{
	int nb_ants;
	int nb_rooms;

	t_room** rooms_by_id;
	t_ant** ants;

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
//t_lemin* parsing(const char* input_file);

// test_generator.c
t_lemin* generate_test(void);

// pathfinding_bfs.c
t_path* bfs_find_path(t_lemin* lemin, int start_id, int end_id);
void reset_room_states(t_lemin* lemin);
t_path* create_path(void);
void add_room_to_path(t_path* path, int room_id);
void free_path(t_path* path);
t_path* reconstruct_path(t_lemin* lemin, int start_id, int end_id);
t_path** find_multiple_paths(t_lemin* lemin, int max_paths);
int count_valid_paths(t_path** paths, int max_paths);
bool paths_inter_rooms(t_path* path1, t_path* path2);
t_path* bfs_find_otherpath(t_lemin* lemin,
							   int start_id,
							   int end_id,
							   t_path** existing_paths,
							   int nb_existing);
bool is_room_in_path(t_path* path, int room_id, bool exclude_endpoints);

// simulation.c
void simulate_ants(t_lemin* lemin);
void init_ants(t_lemin* lemin, t_path** paths, int nb_paths);
void optimize_ants(t_lemin* lemin, t_path** paths, int nb_paths);
t_list* move_ants_one_turn(t_lemin* lemin);
void print_moves(t_list* moves);
void free_moves(t_list* moves);
bool finish_ants(t_lemin* lemin);
int get_room_id_from_path(t_path* path, int position);
int distribution_cost(int* ants_per_path,
									 t_path** paths,
									 int nb_paths);

t_lemin *parse(int fd);
void lemin_print_map_pretty(const t_lemin *lem);
void free_everything_lemin(t_lemin *lemin);

#endif
