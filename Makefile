NAME = lem-in

CXX = cc
CXXFLAGS = -Wall -Wextra -Werror -ggdb3
CXXFLAGS = -ggdb3 -g -fsanitize=address #temporaire pour test 

INC = -I includes/ -I libft/include/

LIBFT_DIR = libft/
LIBFT_A = $(LIBFT_DIR)libft.a

SRCS_DIR = srcs/lemin/
SRCS_FILES = main.c parsing.c

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

.PHONY: all clean fclean re
