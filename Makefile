NAME = lem-in
VISU = visu

CXX = cc
CXXFLAGS = -Wall -Wextra -Werror -ggdb3

INC = -I include/ -I $(LIBFT_DIR)/include -I $(RAYLIB_DIR)
INC_BONUS = -I $(RAYLIB_DIR)

LIBS_DIR = ./libs

LIBFT_DIR = $(LIBS_DIR)/libft/
LIBFT_A = $(LIBFT_DIR)libft.a

RAYLIB_DIR = $(LIBS_DIR)/raylib/src/
RAYLIB_A = $(RAYLIB_DIR)libraylib.a
RAYLIB_DEP = -lGL -lm -lpthread -ldl -lrt -lX11

SRCS_DIR = srcs/
SRCS_FILES = main.c parsing.c pathfinding_bfs.c simulation.c  utils.c

SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))

OBJS_DIR = objs/
OBJS = $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

VISU_SRC = visu.c
VISU_OBJ = objs/visu.o

all: $(NAME)

bonus: $(VISU)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBFT_A) -o $(NAME)

$(VISU): $(VISU_OBJ) $(OBJS_DIR)parsing.o $(OBJS_DIR)pathfinding_bfs.o $(OBJS_DIR)simulation.o $(OBJS_DIR)utils.o $(OBJS_DIR)draw.o $(LIBFT_A) $(RAYLIB_A)
	$(CXX) $(CXXFLAGS) $(VISU_OBJ) $(OBJS_DIR)parsing.o $(OBJS_DIR)pathfinding_bfs.o $(OBJS_DIR)simulation.o $(OBJS_DIR)utils.o $(OBJS_DIR)draw.o $(LIBFT_A) $(RAYLIB_A) $(RAYLIB_DEP) -o $(VISU)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(VISU_OBJ): $(VISU_SRC)
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) bonus

$(RAYLIB_A):
	@$(MAKE) -C $(RAYLIB_DIR)

clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME) $(VISU)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(RAYLIB_DIR) clean

re: fclean all

valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all -q ./$(NAME)

.PHONY: all bonus clean fclean re valgrind
