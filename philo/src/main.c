#include "../inc/philo.h"

int create_forks(t_data_philo **data)
{
  int i;
  i = 0;
  (*data)->forks = calloc(sizeof(pthread_mutex_t), (*data)->n_philo);
  while (i < (*data)->n_philo)
  {
    if (pthread_mutex_init(&(*data)->forks[i], NULL) != 0)
		{
			printf("error initializing forks!\n");
			return (1);
		}
		i++;
  }
  return (0);
}

bool create_philos(t_data_philo **data)
{
  int i;
  i = 0;
  // array di filosofi dentro data
  (*data)->philos = calloc(sizeof(t_philo), (*data)->n_philo);
  // array di pthread 
  (*data)->ids = calloc(sizeof(pthread_t), (*data)->n_philo);
  if (!(*data)->philos || !(*data)->ids)
    return (true);
  while(i < (*data)->n_philo)
  {
    (*data)->philos[i].id = i;
    (*data)->philos[i].data = (*data); 
    i++;
  }
  return (false);
}

int main(int argc, char **argv)
{
  if (argc < 4 || argc > 5)
    return (printf("error\n"), 1);
  t_data_philo *data = NULL;
  parse_input(argc, argv, &data);
  if (create_forks(&data))
    return (free_p(&data, 1));
  if (create_philos(&data))
    return (free_p(&data, 1));
  init_routine(&data);
  // reformatted free function
  free_p(&data, 0);
  return (0);
}
