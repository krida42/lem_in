#include "lemin.h"
#include "raylib.h"

#define CIRCLE_RADIUS 50





void apply_offset_and_margin_to_all_room_positions(t_lemin* lemin, int offset_x, int offset_y, int margin)
{
	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		room->x = room->x * margin + offset_x;
		room->y = room->y * margin + offset_y;
	}
}

void apply_clamping_screen_bounds_to_all_room_positions(t_lemin* lemin, int screen_width, int screen_height, int margin)
{
	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		if (room->x < margin)
			room->x = margin;
		if (room->y < margin)
			room->y = margin;
		if (room->x > screen_width - margin)
			room->x = screen_width - margin;
		if (room->y > screen_height - margin)
			room->y = screen_height - margin;
	}
}

void draw_rooms(t_lemin* lemin)
{
	

	int const circle_radius = CIRCLE_RADIUS;
	int const font_size = 20;

	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		int room_x = room->x;
		int room_y = room->y;

		DrawCircle(room_x, room_y, circle_radius, (Color){ 132, 210, 246, 255 });
		DrawText(room->name, room_x - 10, room_y - 25, font_size, BLACK);
	}
}



void draw_ant_at_position(int ant_id, float x, float y)
{
	int const ant_radius = 18;
	int const font_size = 15;
	
	DrawCircle((int)x, (int)y, ant_radius, (Color){ 246, 132, 210, 255 });
	char *ant_id_str = ft_itoa(ant_id);
	DrawText(ant_id_str, (int)x - 5, (int)y - 5, font_size, BLACK);
	free(ant_id_str);
}


void draw_links(t_lemin* lemin)
{
	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		t_list* link_node = room->links;
		while (link_node)
		{
			int linked_room_id = *(int*)link_node->content;
			t_room* linked_room = lemin->rooms_by_id[linked_room_id];
			draw_line_between_rooms(room, linked_room, (Color){ 200, 200, 200, 255 });
			link_node = link_node->next;
		}
	}
}


void draw_ant_based_on_move(t_lemin* lemin, t_move* move)
{
	int const margin = 1;
	t_room* from_room = NULL;
	t_room* to_room = NULL;


	if (lemin)
	{
		from_room = lemin->rooms_by_id[move->from_room_id];
		to_room = lemin->rooms_by_id[move->to_room_id];
	}

	if (!from_room || !to_room)
		return;

	// Calculate interpolated position
	float from_x = from_room->x * margin;
	float from_y = from_room->y * margin;
	float to_x = to_room->x * margin;
	float to_y = to_room->y * margin;

	float interp_x = from_x + (to_x - from_x) * (move->time_interpolation);
	float interp_y = from_y + (to_y - from_y) * (move->time_interpolation);


	draw_line_between_rooms(from_room, to_room, (Color){ 255, 0, 0, 255 });
	draw_ant_at_position(move->ant_id, interp_x, interp_y);
}

void draw_line_between_rooms(t_room* from, t_room* to, Color color)
{
	int const margin = 1;
	int from_x = from->x * margin;
	int from_y = from->y * margin;
	int to_x = to->x * margin;
	int to_y = to->y * margin;

	int const line_thickness = 4;


	DrawLineEx((Vector2){ from_x, from_y }, (Vector2){ to_x, to_y }, line_thickness, color);
	// printf("Drawing line from (%d, %d) to (%d, %d)\n", from_x, from_y, to_x, to_y);
}


int all_moves_completed(t_list* moves)
{
	t_list* move_node = moves;
	while (move_node)
	{
		t_move* move = (t_move*)move_node->content;
		if (move && move->time_interpolation < 1.0f)
			return 0; // At least one move is not completed
		move_node = move_node->next;
	}
	return 1; // All moves completed
}




void draw_nb_ants_at_start(t_lemin* lemin)
{
	int const font_size = 20;
	char* text = ft_itoa(lemin->nb_ants - lemin->nb_ants_who_left_start);
	DrawText(text, 500, 80, font_size, BLACK);
	free(text);
}

int draw_step_with_time_interpolation(t_lemin* lemin, t_list* current_moves_step, float delta_time)
{

	static int nb_ants_at_start = -1;
	if (nb_ants_at_start == -1 && lemin)
		nb_ants_at_start = lemin->nb_ants;
	if (!lemin || !current_moves_step)
		return 0;

	draw_links(lemin);
	draw_rooms(lemin);
	draw_nb_ants_at_start(lemin);	
	t_list* move_node = current_moves_step;

	if (all_moves_completed(move_node))
		return 1;

	while (move_node)
	{
		t_move* move = (t_move*)move_node->content;
		if (move)
		{
			// Update time interpolation
			move->time_interpolation += delta_time * 1; // Adjust speed
			if (move->time_interpolation > 1.0f) {
				move->time_interpolation = 1.0f;
			}
			draw_ant_based_on_move(lemin, move);
			// printf("Drawing ant %d at interpolation %.2f\n", move->ant_id, move->time_interpolation);
		}
		move_node = move_node->next;
	}


	return 0;
}




int calculate_number_of_ants_who_left_start_in_current_step(t_lemin* lemin, t_list* current_moves_step)
{
	if (!lemin || !current_moves_step)
		return 0;

	int count = 0;
	t_list* move_node = current_moves_step;
	while (move_node)
	{
		t_move* move = (t_move*)move_node->content;
		if (move && move->from_room_id == lemin->start_id && move->to_room_id != lemin->start_id && move->time_interpolation >= 1.0f)
		{
			count++;
		}
		move_node = move_node->next;
	}
	return count;
}