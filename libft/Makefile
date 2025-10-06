NAME = libft.a
SRCS =
OBJS = $(SRCS:.c=.o)
CXX = CXX
CXXFLAGS = -Wall -Wextra -Werror -ggdb3

.c.o:
	$(CXX) $(CXXFLAGS) -c $< -o $(<:.c=.o)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
