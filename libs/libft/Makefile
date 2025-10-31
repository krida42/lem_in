NAME = libft.a

CC = cc
AR = ar rcs
RM = rm -f

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -g3
DEPFLAGS = -MMD -MP

RESET = \033[0m
BOLD = \033[1m
DIM = \033[2m
UNDERLINE = \033[4m
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
GRAY = \033[0;90m

SRCS_FILES = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
			 ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
			 ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
			 ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
			 ft_atoi.c ft_calloc.c ft_strdup.c \
			 ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
			 ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
			 ft_putendl_fd.c ft_putnbr_fd.c \
			 ft_printf.c ft_print_char.c ft_print_str.c ft_print_utils_hex.c \
			 ft_print_ptr.c ft_print_nbr.c ft_print_unsigned.c ft_print_hex.c \
			 get_next_line_utils.c get_next_line.c

SRCS_BONUS_FILES = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
				   ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
				   ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c


SRCS = $(addprefix $(SRC_DIR)/, $(SRCS_FILES))
SRCS_BONUS = $(addprefix $(SRC_DIR)/, $(SRCS_BONUS_FILES))

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
OBJS_BONUS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_BONUS))

DEPS = $(OBJS:.o=.d)
DEPS_BONUS = $(OBJS_BONUS:.o=.d)


all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo "$(GREEN)$(BOLD)✓ Mandatory $(NAME) created successfully!$(RESET)"

bonus: CFLAGS += -DBONUS_FLAG=1
bonus: $(OBJ_DIR) $(OBJS) $(OBJS_BONUS)
	@$(AR) $(NAME) $(OBJS) $(OBJS_BONUS)
	@echo "$(GREEN)$(BOLD)✓ $(NAME) with Bonus parts created successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(YELLOW)Compiling:$(RESET) $<"
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(BLUE)Created directory:$(RESET) $(OBJ_DIR)"

clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		$(RM) -r $(OBJ_DIR); \
		echo "$(MAGENTA)Cleaned object directory:$(RESET) $(OBJ_DIR)"; \
	else \
		echo "$(GRAY)Object directory $(OBJ_DIR) not found. Nothing to clean.$(RESET)"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		$(RM) $(NAME); \
		echo "$(RED)Cleaned library:$(RESET) $(NAME)"; \
	else \
		echo "$(GRAY)Library $(NAME) not found. Nothing to clean.$(RESET)"; \
	fi


re: fclean all

-include $(DEPS)
-include $(DEPS_BONUS)

.PHONY: all clean fclean re bonus
