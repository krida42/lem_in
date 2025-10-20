#include "../../includes/lemin.h"
#include "raylib.h"

void display_lemin(t_lemin* lemin)
{
	ft_printf("Number of ants: %d\n", lemin->nb_ants);
	ft_printf("Number of rooms: %d\n", lemin->nb_rooms);
	ft_printf("Start room ID: %d\n", lemin->start_id);
	ft_printf("End room ID: %d\n", lemin->end_id);
	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		ft_printf("Room %d (%s) links to: ", room->id, room->name);
		t_list* link = room->links;
		while (link)
		{
			int* linked_id = (int*)link->content;
			ft_printf("%d ", *linked_id);
			link = link->next;
		}
		ft_printf("\n");
	}
}






int draw_all(t_lemin* lemin)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

	SetTraceLogLevel(LOG_FATAL); // Disable raylib log output
    InitWindow(screenWidth, screenHeight, "LEMIN Visualization");


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop

	t_list *current_moves_step = lemin->moves_steps;
	if (!current_moves_step)
	{
		printf("No moves to display.\n");
		CloseWindow();
		return (1);
	}
	apply_offset_and_margin_to_all_room_positions(lemin, screenWidth / 4, screenHeight / 4, 100);
	apply_clamping_screen_bounds_to_all_room_positions(lemin, screenWidth, screenHeight, 100);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

			if (!current_moves_step)
			{
				// printf("No more moves to display.\n");
				EndDrawing();
				continue;
			}
			int step_finished =  draw_step_with_time_interpolation(lemin, current_moves_step->content, GetFrameTime());
			if (step_finished) {
				// printf("Step finished, moving to next step.\n");
				int ants_left_start_in_step = calculate_number_of_ants_who_left_start_in_current_step(lemin, current_moves_step->content);
				lemin->nb_ants_who_left_start += ants_left_start_in_step;
				current_moves_step = current_moves_step->next;
				if (!current_moves_step) {
					current_moves_step = lemin->moves_steps; // Loop back to start
				} 
			}

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



int main(int argc, const char* argv[])
{
	if (argc > 1)
	{
		ft_printf("Usage: %s < [input_file]\n", argv[0]);
		return (1);
	}

	//const char* input_file = (argc == 2) ? argv[1] : NULL;

	t_lemin* lemin = parse(0);
	if (!lemin)
		ft_error(
		  "ERROR: Failed to generate test graph.", NULL, __FILE__, __LINE__);

	ft_printf("%d\n", lemin->nb_ants);

	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		if (i == lemin->start_id)
			ft_printf("##start\n");
		if (i == lemin->end_id)
			ft_printf("##end\n");
		ft_printf("%s %d %d\n", room->name, i, i); // x=id, y=id
	}

	for (int i = 0; i < lemin->nb_rooms; i++)
	{
		t_room* room = lemin->rooms_by_id[i];
		t_list* link = room->links;
		while (link)
		{
			int linked_id = *(int*)link->content;
			if (i < linked_id)
				ft_printf(
				  "%s-%s\n", room->name, lemin->rooms_by_id[linked_id]->name);
			link = link->next;
		}
	}

	ft_printf("\n");

	simulate_ants(lemin);

	draw_all(lemin);

	if (lemin)
	{
		ft_free_lemin(lemin);
		lemin = NULL;
	}
	return (0);
}
