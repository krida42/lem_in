NAME = lem-in

CXX = cc
CXXFLAGS = -Wall -Wextra -Werror

INC = -I includes/ -I libft/

LIBFT_DIR = libft/
LIBFT_A = $(LIBFT_DIR)libft.a

SRCS_DIR = srcs/lemin/
SRCS_FILES = main.c parsing.c pathfinding_bfs.c test_generator.c utils.c

SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))

OBJS_DIR = objs/
OBJS = $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBFT_A) -o $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) bonus

clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

valgrind: re
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re
