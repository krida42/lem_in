#ifndef LEMIN_H
#define LEMIN_H

#include "../libs/libft/include/get_next_line.h"
#include "../libs/libft/include/libft.h"
#include <limits.h>	 // INT_MAX
#include <stdbool.h> // true, false
#include <unistd.h>	 // write

#include "../libs/raylib/src/raylib.h"

#define ERROR -1
#define SUCCESS 0

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

enum RoomType
{
	ROOM_TYPE_UNKNOWN,
	ROOM_START,
	ROOM_END
};

enum ParsingState
{
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

	int old_x; // For visualization
	int old_y; // For visualization

	bool is_on_path;
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
	Color color;
} t_ant;

typedef struct s_move
{
	int ant_id;
	int from_room_id;
	int to_room_id;
	char* to_room_name;
	float time_interpolation;
} t_move;

typedef struct s_lemin
{
	int nb_ants;
	int nb_rooms;

	t_room** rooms_by_id;
	t_ant** ants;

	int start_id; // ID de ##start
	int end_id;	  // ID de ##end

	int** capacity;
	int** used;

	t_list* moves_steps;
	int nb_ants_who_left_start;
} t_lemin;

// utils.c
void ft_error(char* msg, t_lemin* lemin, const char* file, int line);
void ft_free_lemin(t_lemin* lemin);
int ft_strcmp(const char* s1, const char* s2);
char* get_next_line_wnl(int fd);
void free_split(char** split_array);
void free_move(t_move* move);
void free_moves(t_list* moves);
t_move* create_move(int ant_id,
					int from_room_id,
					int to_room_id,
					char* to_room_name);

// parsing.c
t_lemin* parse(int fd);
bool is_str_pnumber(const char* str);
bool is_str_integer(const char* str);
bool is_room_name_valid(const char* name);
bool is_comment(const char* line);
bool is_start_command(const char* line);
bool is_end_command(const char* line);
bool is_room_definition(const char* line);
t_room* parse_room(const char* line);
bool is_room_name(t_lemin* lemin, const char* name);
void free_room(t_room* room);
void room_to_lemin(t_lemin* lemin, t_room* room, char* line);
enum ParsingState parsing_rooms(t_lemin* lemin,
								char* line,
								enum RoomType* next_room_type);
char** parse_link(const char* line);
int parse_ants(const char* line);
enum ParsingState process_parsing_ants(t_lemin* lemin, char* line);
enum ParsingState process_parsing_links(t_lemin* lemin, char* line);
int get_room_id(t_lemin* lemin, const char* name);
void integrate_link_rooms(t_lemin* lemin,
						  int parent_room_id,
						  int child_room_id);

// pathfinding_bfs.c
void reset_room_states(t_lemin* lemin);
t_path* create_path();
void free_path(t_path* path);
t_path* bfs_find_path(t_lemin* lemin, int start_id, int end_id);
t_path** find_multiple_paths(t_lemin* lemin, int max_paths);
int count_valid_paths(t_path** paths, int max_paths);

// simulation.c
void optimize_ants(t_lemin* lemin, t_path** paths, int nb_paths);
void simulate_ants(t_lemin* lemin);

// draw.c
void draw_rooms(t_lemin* lemin);
int draw_step(t_lemin* lemin, t_list* current_moves_step, float time);
void apply_offset(t_lemin* lemin, int offset_x, int offset_y, int margin);
void apply_clamping(t_lemin* lemin,
					int screen_width,
					int screen_height,
					int margin);
void draw_line(t_room* from, t_room* to, Color color);
int run_visualization(t_lemin* lemin);

t_list* parse_moves(t_lemin* lemin, int fd);

#endif
