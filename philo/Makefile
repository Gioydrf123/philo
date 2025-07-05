CC = cc
CFLAGS = -pthread -Wall -Wextra -Werror -g
NAME = philo
SRCS = ./src/main.c ./src/routine.c ./src/utils.c ./src/parsing.c \
		./src/monitor.c ./src/print.c ./src/time.c

all :
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
	rm $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
