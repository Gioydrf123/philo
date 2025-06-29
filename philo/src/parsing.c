#include "../inc/philo.h"

void parse_input(int argc, char **argv, t_data_philo **data)
{
  *data = calloc(sizeof(t_data_philo), 1);
  (*data)->n_philo = atoi(argv[1]);
  (*data)->tt_die = atoi(argv[2]);
  (*data)->tt_eat = atoi(argv[3]);
  (*data)->tt_sleep = atoi(argv[4]);
  if (argc == 6)
    (*data)->max_eat = atoi(argv[5]);
  else
    (*data)->max_eat = -1;
  printf("numero filosofi : %d\n",(*data)->n_philo);
  printf("time to die : %d\n",(*data)->tt_die);
  printf("time to eat : %d\n",(*data)->tt_eat);
  printf("time to sleep : %d\n",(*data)->tt_sleep);
  (*data)->simulation_running = true;
}
