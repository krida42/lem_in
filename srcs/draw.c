#include "../include/lemin.h"
#include "../libs/raylib/src/raylib.h"
#include <math.h>

#define CIRCLE_RADIUS 50

void apply_offset(t_lemin* lemin, int offset_x, int offset_y, int margin)
{
	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		room->x = room->x * margin + offset_x;
		room->y = room->y * margin + offset_y;
	}
}

void apply_clamping(t_lemin* lemin,
					int screen_width,
					int screen_height,
					int margin)
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

		Color room_color = (Color){ 132, 210, 246, 255 };

		if (room->id == lemin->start_id)
			room_color = LIME; // Green for start
		else if (room->id == lemin->end_id)
			room_color = VIOLET; // Purple for end

		DrawCircle(room_x, room_y, circle_radius, room_color);
		DrawCircleLines(room_x, room_y, circle_radius, DARKBROWN); // Outline
		DrawText(room->name, room_x - 10, room_y - 25, font_size, BLACK);
	}
}

void draw_ant_at_position(int ant_id, float x, float y, Color color)
{
	int const ant_radius = 18;
	int const font_size = 15;

	DrawCircle((int)x, (int)y, ant_radius, color);
	char* ant_id_str = ft_itoa(ant_id);
	DrawText(ant_id_str, (int)x - 5, (int)y - 5, font_size, BLACK);
	free(ant_id_str);
}

void draw_stationary_ants(t_lemin* lemin)
{
	for (int i = 0; i < lemin->nb_ants; i++)
	{
		t_ant* ant = lemin->ants[i];
		if (!ant)
			continue;

		if (ant->current_room_id != lemin->start_id &&
			ant->current_room_id != lemin->end_id)
		{
			t_room* room = lemin->rooms_by_id[ant->current_room_id];
			if (room)
			{
				int const ant_radius = 15;
				int const font_size = 12;

				int offset_x = (ant->id % 3 - 1) * 8; // -8, 0, or 8
				int offset_y = (ant->id % 2) * 15;	  // 0 or 15

				DrawCircle(room->x + offset_x,
						   room->y + offset_y,
						   ant_radius,
						   ant->color);
				char* ant_id_str = ft_itoa(ant->id);
				DrawText(ant_id_str,
						 room->x + offset_x - 4,
						 room->y + offset_y - 4,
						 font_size,
						 BLACK);
				free(ant_id_str);
			}
		}
	}
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
			if (i < linked_room_id)
			{
				t_room* linked_room = lemin->rooms_by_id[linked_room_id];
				draw_line(room, linked_room, (Color){ 200, 200, 200, 255 });
			}
			link_node = link_node->next;
		}
	}
}

void draw_ant_based_on_move(t_lemin* lemin, t_move* move)
{
	t_room* from_room = NULL;
	t_room* to_room = NULL;

	if (lemin)
	{
		from_room = lemin->rooms_by_id[move->from_room_id];
		to_room = lemin->rooms_by_id[move->to_room_id];
	}

	if (!from_room || !to_room)
		return;

	Color ant_color = PINK;
	if (lemin && move->ant_id >= 1 && move->ant_id <= lemin->nb_ants)
	{
		t_ant* ant = lemin->ants[move->ant_id - 1];
		if (ant)
			ant_color = ant->color;
	}

	float from_x = from_room->x;
	float from_y = from_room->y;
	float to_x = to_room->x;
	float to_y = to_room->y;

	float t = move->time_interpolation;
	float eased_t = sinf(t * PI / 2.0f);

	float interp_x = from_x + (to_x - from_x) * eased_t;
	float interp_y = from_y + (to_y - from_y) * eased_t;

	draw_line(from_room, to_room, ant_color);
	draw_ant_at_position(move->ant_id, interp_x, interp_y, ant_color);
}

void draw_line(t_room* from, t_room* to, Color color)
{
	int from_x = from->x;
	int from_y = from->y;
	int to_x = to->x;
	int to_y = to->y;

	int const line_thickness = 4;

	DrawLineEx((Vector2){ from_x, from_y },
			   (Vector2){ to_x, to_y },
			   line_thickness,
			   color);
}

int all_moves_completed(t_list* moves)
{
	t_list* move_node = moves;
	while (move_node)
	{
		t_move* move = (t_move*)move_node->content;
		if (move && move->time_interpolation < 1.0f)
			return 0;
		move_node = move_node->next;
	}
	return 1;
}

void draw_nb_ants_at_start(t_lemin* lemin)
{
	int const font_size = 20;
	char* text = ft_itoa(lemin->nb_ants - lemin->nb_ants_who_left_start);
	DrawText(text, 500, 80, font_size, BLACK);
	free(text);
}

int draw_step(t_lemin* lemin, t_list* current_moves_step, float delta_time)
{

	static int nb_ants_at_start = -1;
	if (nb_ants_at_start == -1 && lemin)
		nb_ants_at_start = lemin->nb_ants;
	if (!lemin || !current_moves_step)
		return 0;

	draw_links(lemin);
	draw_rooms(lemin);
	draw_stationary_ants(lemin);
	draw_nb_ants_at_start(lemin);
	t_list* move_node = current_moves_step;

	if (all_moves_completed(move_node))
		return 1;

	while (move_node)
	{
		t_move* move = (t_move*)move_node->content;
		if (move)
		{
			move->time_interpolation += delta_time * 1;
			if (move->time_interpolation > 1.0f)
				move->time_interpolation = 1.0f;
			draw_ant_based_on_move(lemin, move);
		}
		move_node = move_node->next;
	}

	return 0;
}

int calculate_number_of_ants_who_left_start_in_current_step(
  t_lemin* lemin,
  t_list* current_moves_step)
{
	if (!lemin || !current_moves_step)
		return 0;

	int count = 0;
	t_list* move_node = current_moves_step;
	while (move_node)
	{
		t_move* move = (t_move*)move_node->content;
		if (move && move->from_room_id == lemin->start_id &&
			move->to_room_id != lemin->start_id &&
			move->time_interpolation >= 1.0f)
		{
			count++;
		}
		move_node = move_node->next;
	}
	return count;
}

int run_visualization(t_lemin* lemin)
{
	const int screenWidth = 1920;
	const int screenHeight = 1080;

	if (!getenv("DISPLAY"))
	{
		ft_printf("No display available, cannot run visualizer\n");
		return 0;
	}

	SetTraceLogLevel(LOG_NONE);
	InitWindow(screenWidth, screenHeight, "LEMIN Visualization");

	if (!IsWindowReady())
	{
		ft_printf("No display available, cannot run visualizer\n");
		return 0;
	}

	SetTargetFPS(60);
	t_list* current_moves_step = lemin->moves_steps;
	if (!current_moves_step)
	{
		CloseWindow();
		return (1);
	}
	apply_offset(lemin, screenWidth / 4, screenHeight / 4, 100);
	apply_clamping(lemin, screenWidth, screenHeight, 100);

	bool simulation_finished = false;
	bool paused = false;
	float speed_multiplier = 1.0f;
	int current_turn = 0;

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_SPACE))
		{
			paused = !paused;
		}

		if (IsKeyPressed(KEY_RIGHT))
		{
			speed_multiplier *= 1.5f;
			if (speed_multiplier > 4.0f)
				speed_multiplier = 4.0f;
		}
		if (IsKeyPressed(KEY_LEFT))
		{
			speed_multiplier /= 1.5f;
			if (speed_multiplier < 0.25f)
				speed_multiplier = 0.25f;
		}

		if (IsKeyPressed(KEY_R))
		{
			t_list* step = lemin->moves_steps;
			while (step)
			{
				t_list* move_node = (t_list*)step->content;
				while (move_node)
				{
					t_move* move = (t_move*)move_node->content;
					if (move)
						move->time_interpolation = 0.0f;
					move_node = move_node->next;
				}
				step = step->next;
			}
			current_moves_step = lemin->moves_steps;
			lemin->nb_ants_who_left_start = 0;
			simulation_finished = false;
			current_turn = 0;
			paused = false;
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawRectangle(10, 10, 340, 160, (Color){ 50, 50, 50, 200 });
		DrawText("LEMIN Visualization", 20, 20, 20, RAYWHITE);

		DrawText("Turn: ", 20, 50, 18, RAYWHITE);
		char* turn_str = ft_itoa(current_turn);
		DrawText(turn_str, 80, 50, 18, RAYWHITE);
		free(turn_str);

		DrawText("Ants at end: ", 20, 75, 18, RAYWHITE);
		char* ants_at_end_str = ft_itoa(lemin->nb_ants_who_left_start);
		DrawText(ants_at_end_str, 140, 75, 18, RAYWHITE);
		DrawText(
		  " / ", 140 + MeasureText(ants_at_end_str, 18), 75, 18, RAYWHITE);
		char* total_ants_str = ft_itoa(lemin->nb_ants);
		DrawText(total_ants_str,
				 140 + MeasureText(ants_at_end_str, 18) + 20,
				 75,
				 18,
				 RAYWHITE);
		free(ants_at_end_str);
		free(total_ants_str);

		char* status_text = paused ? "[PAUSED]" : "[RUNNING]";
		DrawText(status_text, 20, 100, 16, paused ? YELLOW : LIME);

		DrawText("Speed: ", 20, 120, 16, RAYWHITE);
		if (speed_multiplier >= 3.5f)
			DrawText("4x", 90, 120, 16, RAYWHITE);
		else if (speed_multiplier >= 2.0f)
			DrawText("2.67x", 90, 120, 16, RAYWHITE);
		else if (speed_multiplier >= 1.3f)
			DrawText("1.78x", 90, 120, 16, RAYWHITE);
		else if (speed_multiplier >= 0.9f)
			DrawText("1x", 90, 120, 16, RAYWHITE);
		else if (speed_multiplier >= 0.6f)
			DrawText("0.67x", 90, 120, 16, RAYWHITE);
		else if (speed_multiplier >= 0.3f)
			DrawText("0.44x", 90, 120, 16, RAYWHITE);
		else
			DrawText("0.25x", 90, 120, 16, RAYWHITE);

		DrawText(
		  "SPACE: Pause | R: Restart | <- ->: Speed", 20, 140, 14, LIGHTGRAY);

		if (!simulation_finished && current_moves_step)
		{
			float delta = paused ? 0.0f : GetFrameTime() * speed_multiplier;
			int step_finished =
			  draw_step(lemin, current_moves_step->content, delta);
			if (step_finished)
			{
				int ants_left_start_in_step =
				  calculate_number_of_ants_who_left_start_in_current_step(
					lemin, current_moves_step->content);
				lemin->nb_ants_who_left_start += ants_left_start_in_step;
				current_moves_step = current_moves_step->next;
				current_turn++;
				if (!current_moves_step)
					simulation_finished = true;
			}
		}
		else if (simulation_finished)
		{
			draw_links(lemin);
			draw_rooms(lemin);
			draw_stationary_ants(lemin);
			draw_nb_ants_at_start(lemin);

			int msg_width = 400;
			int msg_height = 60;
			int msg_x = (screenWidth - msg_width) / 2;
			int msg_y = 50;
			DrawRectangle(
			  msg_x, msg_y, msg_width, msg_height, (Color){ 34, 139, 34, 220 });
			DrawRectangleLines(msg_x, msg_y, msg_width, msg_height, DARKGREEN);
			DrawText(
			  "SIMULATION COMPLETE!", msg_x + 50, msg_y + 10, 20, RAYWHITE);
			DrawText(
			  "Press 'R' to restart", msg_x + 100, msg_y + 35, 16, RAYWHITE);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
