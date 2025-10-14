#include "lemin.h"

int ft_strcmp(const char *s1, const char *s2) {
	return ft_strncmp(s1, s2, INT_MAX);
}



char *get_next_line_without_newline(int fd) {
	char *str = get_next_line(fd);

	if (!str) return NULL;
	char *newline_pos = ft_strchr(str, '\n');
	if (newline_pos) *newline_pos = '\0';
	return str;
	
}


void free_split(char **split_array)
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

bool is_str_positive_number(const char *str) {
	if (!str || *str == '\0') return false;
	while (*str) {
		if (*str < '0' || *str > '9') return false;
		str++;
	}
	return true;
}

bool is_room_name_valid(const char *name) {
	if (!name || *name == '\0') return false;
	if (name[0] == 'L' || name[0] == '#') return false; // Invalid starting char
	while (*name) {
		if (*name == ' ' || *name == '-') return false; // Invalid chars
		name++;
	}
	return true;
}

bool is_comment(const char *line) {
	return line[0] == '#' && line[1] != '#';
}


bool is_start_command(const char *line) {
	return ft_strcmp(line, "##start") == 0;
}
bool is_end_command(const char *line) {
	return ft_strcmp(line, "##end") == 0;
}



void print_lemin(t_lemin *lemin) {
	if (!lemin) {
		printf("Lemin is NULL\n");
		return;
	}
	printf("- - - - - - - PRINT LEMIN PARSED - - - START- - \n");
	printf("Number of ants: %d\n", lemin->nb_ants);
	printf("Number of rooms: %d\n", lemin->nb_rooms);
	printf("Start room name: '%s', ID: (%d)\n", lemin->rooms_by_id[lemin->start_id]->name, lemin->start_id);
	printf("End room name: '%s', ID: (%d)\n", lemin->rooms_by_id[lemin->end_id]->name, lemin->end_id);
	printf("\nRooms details:\n");

	// print all the rooms beaitfully with there coordinates and links, I want it to be pretty, with color
	// I want it to be very visual with arrows and everything
	for (int i = 0; i < lemin->nb_rooms; i++) {
		t_room *room = lemin->rooms_by_id[i];
		if (!room) {
			printf("Room %d is NULL\n", i);
			continue;
		}
		printf("Room Name: '%s', ID: (%d), Coordinates: (%d, %d), Links to: ", room->name, room->id, room->x, room->y);
		if (!room->links) {
			printf("No links\n");
		} else {
			t_list *link = room->links;
			while (link) {
				int *linked_id = (int *)link->content;
				printf("'%s'  ", lemin->rooms_by_id[*linked_id]->name);
				link = link->next;
			}
			printf("\n");
		}
	}




	printf("- - - - - - - PRINT LEMIN PARSED - - - END- - \n");
}




// Not to used functions, just for reference
bool is_room_definition(const char *line) {
	// A room definition typically looks like: "RoomName X Y"
	// where RoomName is a string without spaces, and X, Y are integers.
	int i = 0;
	
	while (line[i] && ft_isalnum(line[i]))
		i++;
	if (line[i] != ' ')
		return false; // No space after room name
	i++; // Skip space after room name


	if (!ft_isdigit(line[i]))
		return false; // No X coordinate
	while (line[i] && ft_isdigit(line[i]))
		i++;
	if (line[i] != ' ')
		return false; // No space after X coordinate
	i++; // Skip space after X coordinate


	if (!ft_isdigit(line[i]))
		return false; // No Y coordinate
	while (line[i] && ft_isdigit(line[i]))
		i++;
	
	return line[i] == '\0'; // Should end after Y coordinate
		
}


t_room *parse_room(const char *line) {
	if (!line)
		return NULL;
	char **parts = ft_split(line, ' ');
	if (!parts || !parts[0] || !parts[1] || !parts[2] || parts[3] != NULL) {
		// Invalid room definition
		free_split(parts);
		return NULL;
	}
	if (!is_room_name_valid(parts[0]) || !is_str_positive_number(parts[1]) || !is_str_positive_number(parts[2])) {
		free_split(parts);
		return NULL;
	}

	t_room *room = malloc(sizeof(t_room));
	if (!room) {
		free_split(parts);
		return NULL;
	}
	room->name = ft_strdup(parts[0]);
	room->x = ft_atoi(parts[1]);
	room->y = ft_atoi(parts[2]);

	// For now, set default values
	room->id = -1;
	room->links = NULL;
	room->visited = false;
	room->parent_id = -1;
	free_split(parts);
	return room;

}



bool is_room_name_already_used(t_lemin *lemin, const char *name) {
	for (int i = 0; i < lemin->nb_rooms; i++) {
		if (ft_strcmp(lemin->rooms_by_id[i]->name, name) == 0) {
			return true;
		}
	}
	return false;
}

void free_room(t_room *room) {
	if (!room) return;
	if (room->name) free(room->name);
	if (room->links) ft_lstclear(&room->links, free);
	free(room);
}


void clean_quit_if_room_null(t_lemin *lemin, t_room *room, char *line) {
	if (!room) {
		free(line);
		ft_error("ERROR: Invalid start room definition.", lemin, __FILE__, __LINE__);
	}
}

void free_and_quit(t_lemin *lemin, t_room *room, char *line) {
	free(line);
	free_room(room);
	ft_error("ERROR, free_and_quit", lemin, __FILE__, __LINE__);
}

void free_everything_lemin(t_lemin *lemin) {
	if (lemin) {
		if (lemin->rooms_by_id) {
			for (int i = 0; i < lemin->nb_rooms; i++) {
				if (lemin->rooms_by_id[i]) {
					free_room(lemin->rooms_by_id[i]);
				}
			}
			free(lemin->rooms_by_id);
		}
		free(lemin);
	}
}


void clean_quit_if_room_name_already_used(t_lemin *lemin, t_room *room, char *line) {
	if (is_room_name_already_used(lemin, room->name)) {
		free(line);
		free_room(room);
		ft_error("ERROR: Room name already used", lemin, __FILE__, __LINE__);
	}
}


void integrate_room_to_lemin(t_lemin *lemin, t_room *room, char *line) {

	clean_quit_if_room_name_already_used(lemin, room, line);
	t_room **new_rooms_array = realloc(lemin->rooms_by_id, sizeof(t_room *) * (lemin->nb_rooms + 1));
	if (!new_rooms_array)
		free_and_quit(lemin, room, line);
	room->id = lemin->nb_rooms;
	lemin->rooms_by_id = new_rooms_array;
	lemin->rooms_by_id[room->id] = room;
	lemin->nb_rooms++;
}

void integrate_link_to_rooms(t_lemin *lemin, int parent_room_id, int child_room_id) {
	if (parent_room_id < 0 || parent_room_id >= lemin->nb_rooms ||
		child_room_id < 0 || child_room_id >= lemin->nb_rooms) {
		ft_error("ERROR: Invalid room ID in link.", lemin, __FILE__, __LINE__);
	}
	int *child_id_ptr = malloc(sizeof(int));
	if (!child_id_ptr)
		ft_error("ERROR: Malloc failed for link ID.", lemin, __FILE__, __LINE__);
	*child_id_ptr = child_room_id;
	t_list *new_link = ft_lstnew(child_id_ptr);
	if (!new_link) {
		free(child_id_ptr);
		ft_error("ERROR: Malloc failed for link node.", lemin, __FILE__, __LINE__);
	}
	ft_lstadd_back(&lemin->rooms_by_id[parent_room_id]->links, new_link);
}


int get_room_id_by_name(t_lemin *lemin, const char *name) {
	for (int i = 0; i < lemin->nb_rooms; i++) {
		if (ft_strcmp(lemin->rooms_by_id[i]->name, name) == 0) {
			return lemin->rooms_by_id[i]->id;
		}
	}
	return -1; // Not found
}





// Return next room type to expect
enum ParsingState process_parsing_rooms(t_lemin *lemin, char *line, enum RoomType *next_room_type) {
	
	t_room *room = parse_room(line);

	enum RoomType curr_room_type = *next_room_type;
	
	if (curr_room_type == ROOM_START) {
		clean_quit_if_room_null(lemin, room, line);
		lemin->start_id = lemin->nb_rooms;
	} else if (curr_room_type == ROOM_END) {
		clean_quit_if_room_null(lemin, room, line);	
		lemin->end_id = lemin->nb_rooms;
	}
	*next_room_type = ROOM_TYPE_UNKNOWN; // Reset next room type expectation

	
	if (room)
		integrate_room_to_lemin(lemin, room, line);
	else if (is_start_command(line))
		*next_room_type = ROOM_START; // Next room should be start
	else if (is_end_command(line))
		*next_room_type = ROOM_END;
	else
		return PARSING_LINKS; // Switch to parsing links if not a room or command

	
	return PARSING_ROOMS; // Continue parsing rooms
}

// Returns array of two room names
// or NULL if the link definition is invalid
// "RoomA-RoomB" -> [RoomA, RoomB]
char **parse_link(const char *line) {
	char **parts = ft_split(line, '-');
	if (!parts || !parts[0] || !parts[1] || parts[2] != NULL) {
		free_split(parts);
		return NULL; // Invalid link definition
	}
	return parts;
}


// Give the number of ants from a line
// Returns -1 if invalid
int parse_ants(const char *line) {
	if (!is_str_positive_number(line)) {
		printf("line is not a positive number,  ");
		printf("line: '%s'\n", line);
		return -1;
	}
	int ants = ft_atoi(line);
	if (ants <= 0) {
		printf("ants <= 0\n");
		return -1;
	}
	
	return ants;
}


enum ParsingState process_parsing_ants(t_lemin *lemin, char *line) {
	int ants = parse_ants(line);
	if (ants == -1) {
		free(line);
		ft_error("ERROR: Invalid number of ants.", lemin, __FILE__, __LINE__);
	}
	lemin->nb_ants = ants;
	return PARSING_ROOMS; // Switch to parsing rooms
}

enum ParsingState process_parsing_links(t_lemin *lemin, char *line) {
	char **link_rooms_name = parse_link(line);

	if (!link_rooms_name) {
		free(line);
		ft_error("ERROR: Invalid link definition.", lemin, __FILE__, __LINE__);
	}

	int room_parent_id = get_room_id_by_name(lemin, link_rooms_name[0]);
	int room_child_id = get_room_id_by_name(lemin, link_rooms_name[1]);
	free_split(link_rooms_name);
	if (room_parent_id == -1 || room_child_id == -1) {
		free(line);
		ft_error("ERROR: Link references unknown room.", lemin, __FILE__, __LINE__);
	}
	integrate_link_to_rooms(lemin, room_parent_id, room_child_id);

	return PARSING_LINKS; // Continue parsing links
}



t_lemin *parse(int fd) {


	// parsing state

	int parsing_state = PARSING_ANTS;

	t_lemin *lemin = malloc(sizeof(t_lemin));
	lemin->nb_ants = 0;
	lemin->nb_rooms = 0;
	lemin->rooms_by_id = NULL;
	lemin->start_id = -1;
	lemin->end_id = -1;

	printf("Initial lemin state: nb_ants=%d, nb_rooms=%d, start_id=%d, end_id=%d\n",
		   lemin->nb_ants, lemin->nb_rooms, lemin->start_id, lemin->end_id);


	enum RoomType next_room_type = ROOM_TYPE_UNKNOWN;

	char *line = get_next_line_without_newline(fd);
	while (line) {

		if (is_comment(line)) {
			free(line);
			line = get_next_line_without_newline(fd);
			continue;
		}
		
		if (parsing_state == PARSING_ANTS) {
			parsing_state = process_parsing_ants(lemin, line);
		} else if (parsing_state == PARSING_ROOMS) {
			parsing_state = process_parsing_rooms(lemin, line, &next_room_type);
		}

		if (parsing_state == PARSING_LINKS) {
			parsing_state = process_parsing_links(lemin, line);
		}


		free(line);
		line = get_next_line_without_newline(fd);

	}

	print_lemin(lemin);
	

	return lemin;
}