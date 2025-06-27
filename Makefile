CC = cc
CFLAGS = -g -pthread
#  -Wall -Wextra -Werror -g
NAME = philo
SRCS = ./src/main.c

all : $(NAME)

$(NAME) : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
	rm $(NAME)

fclean: clean

re: fclean all

# .PHONYall clean fclean re

