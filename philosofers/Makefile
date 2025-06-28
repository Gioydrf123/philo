CC = cc
CFLAGS = -g -pthread
#  -Wall -Wextra -Werror -g
NAME = philo
SRCS = ./src/main.c

all :
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
	rm $(NAME)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
#per pulire le schifezze
clean:
	rm $(NAME)

fclean: clean

re: fclean all

# .PHONYall clean fclean re

