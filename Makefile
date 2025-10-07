NAME = lem-in
NAME_BONUS = visu

CXX = cc
CXXFLAGS = -Wall -Wextra -Werror -ggdb3

INC = -I includes/ -I $(LIBFT_DIR) -I $(RAYLIB_DIR)
INC_BONUS = -I $(RAYLIB_DIR)

LIBFT_DIR = libs/libft/
LIBFT_A = $(LIBFT_DIR)libft.a

RAYLIB_DIR = libs/raylib/src/
RAYLIB_A = $(RAYLIB_DIR)libraylib.a

SRCS_DIR = srcs/lemin/
SRCS_FILES = main.c parsing.c pathfinding_bfs.c simulation.c test_generator.c utils.c

SRCS_BONUS_DIR = srcs/visu/
SRCS_BONUS_FILES = main_visu.c

SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))
SRCS_BONUS = $(addprefix $(SRCS_BONUS_DIR), $(SRCS_BONUS_FILES))

OBJS_DIR = objs/
OBJS = $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))
OBJS_BONUS = $(patsubst $(SRCS_BONUS_DIR)%.c, $(OBJS_DIR)bonus_%.o, $(SRCS_BONUS))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBFT_A) -o $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS) $(RAYLIB_A)
	$(CXX) $(CXXFLAGS) $(OBJS_BONUS) $(RAYLIB_A) -lm -o $(NAME_BONUS)

$(OBJS_DIR)bonus_%.o: $(SRCS_BONUS_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INC_BONUS) -c $< -o $@

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) bonus

$(RAYLIB_A):
	@$(MAKE) -C $(RAYLIB_DIR)

clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(RAYLIB_DIR) clean

re: fclean all

valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all -q ./$(NAME)

run: $(NAME)
	./$(NAME)

.PHONY: all bonus clean fclean re valgrind run
