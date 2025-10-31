NAME := lem-in
VISU := visu

SRCS_FILES := parsing.c pathfinding_bfs.c simulation.c utils.c
VISU_FILES := visu.c draw.c

SRCS_DIR := srcs/
OBJS_DIR := objs/
LIBS_DIR := libs/

LIBFT_DIR := $(LIBS_DIR)libft/
LIBFT_A := $(LIBFT_DIR)libft.a

RAYLIB_DIR := $(LIBS_DIR)raylib/src/
RAYLIB_A := $(RAYLIB_DIR)libraylib.a
RAYLIB_LIBS := -lGL -lm -lpthread -ldl -lrt -lX11

CC := cc
CFLAGS := -Wall -Wextra -Werror -g3 -MMD -MP

INCLUDES := -I include/ -I $(LIBFT_DIR)include

LDLIBS_MAND := -L$(LIBFT_DIR) -lft
LDLIBS_BONUS := -L$(RAYLIB_DIR) -lraylib $(RAYLIB_LIBS)

SHARED_SRCS := $(addprefix $(SRCS_DIR), $(SRCS_FILES))
MAIN_SRC := $(SRCS_DIR)main.c
VISU_SRCS := $(addprefix $(SRCS_DIR), $(VISU_FILES))

SHARED_OBJS := $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SHARED_SRCS))
MAIN_OBJ := $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(MAIN_SRC))
VISU_OBJS := $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(VISU_SRCS))

OBJS_MAND := $(MAIN_OBJ) $(SHARED_OBJS)
OBJS_BONUS := $(VISU_OBJS) $(SHARED_OBJS)

DEPS := $(OBJS_MAND:.o=.d) $(OBJS_BONUS:.o=.d)

all: $(NAME)

bonus: $(VISU)

$(NAME): $(OBJS_MAND) $(LIBFT_A)
	@echo "[\033[0;32mLD\033[0m] linking $@..."
	$(CC) $^ $(LDLIBS_MAND) -o $@

$(VISU): $(OBJS_BONUS) $(LIBFT_A) $(RAYLIB_A)
	@echo "[\033[0;32mLD\033[0m] linking $@ (bonus)..."
	$(CC) $^ $(LDLIBS_MAND) $(LDLIBS_BONUS) -o $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	@echo "[\033[0;34mCC\033[0m] compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(VISU_OBJS): INCLUDES += -I $(RAYLIB_DIR)

$(LIBFT_A):
	@echo "[\033[0;33mMAKE\033[0m] building libft..."
	@$(MAKE) -s -C $(LIBFT_DIR) bonus

$(RAYLIB_A):
	@echo "[\033[0;33mMAKE\033[0m] building raylib..."
	@$(MAKE) -s -C $(RAYLIB_DIR)

-include $(DEPS)

clean:
	@echo "[\033[0;31mCLEAN\033[0m] removing object files..."
	@rm -rf $(OBJS_DIR)
	@echo "[\033[0;31mCLEAN\033[0m] cleaning external libraries..."
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@$(MAKE) -s -C $(RAYLIB_DIR) clean

fclean: clean
	@echo "[\033[0;31mFCLEAN\033[0m] removing executables..."
	@rm -f $(NAME) $(VISU)
	@echo "[\033[0;31mFCLEAN\033[0m] full cleaning libft..."
	@$(MAKE) -s -C $(LIBFT_DIR) fclean

re: fclean all

rebonus: fclean bonus

.PHONY: all bonus clean fclean re rebonus
