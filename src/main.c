#include "../inc/philo.h"


// long get_timestamp(struct timeval *time_start)
// {
//   struct timeval now;
//   gettimeofday(&now, NULL);
//   return (now.tv_sec - start_time->tv_sec) * 1000 + (now.tv_usec - start_time->tv_usec) / 1000;
// }

// void	distribute_forks(t_controller *controller)
// {
// 	int	i;

// 	i = 0;
// 	while (i < controller->num_of_philos)
// 	{
// 		if (i == 0)
// 			controller->philos[i].fork_r
// 				= &controller->forks[controller->num_of_philos - 1];
// 		else
// 			controller->philos[i].fork_r = &controller->forks[i - 1];
// 		controller->philos[i].fork_l = &controller->forks[i];
// 		i++;
// 	}
// 	init_routine(controller);
// }

void  free_p(t_data_philo **data)
{
  if ((*data)->forks)
    free((*data)->forks);
  if ((*data)->philos)
    free((*data)->philos);
  if ((*data)->ids)
    free((*data)->ids);
  if ((*data))
    free((*data));
}

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
}

bool create_philos(t_data_philo **data)
{
  int i;
  i = 0;
  (*data)->philos = calloc(sizeof(t_philo), (*data)->n_philo);
  (*data)->ids = calloc(sizeof(pthread_t), (*data)->n_philo);
  if (!(*data)->philos || !(*data)->ids)
  {
    return (true);
  }
  while(i < (*data)->n_philo)
  {
    (*data)->philos[i].id = i;
    i++;
  }
  return (false);
}

void *philosophers_routine(void *philo_pointer)
{
  printf("ciao1\n");
}

int init_routine(t_data_philo **data)
{
  int i;
  i = 0;
  while (i < (*data)->n_philo)
  {
    if (pthread_create(&(*data)->ids[i], NULL, &philosophers_routine, &(*data)->philos[i]))
      return printf("error in thread creation\n");
    sleep(1);
    i++;
  }
  while (i-- > 0)
		pthread_join((*data)->ids[i], NULL);
}

int main(int argc, char **argv)
{
  if (argc < 4 || argc > 5)
  {
    printf("error\n");
    return (1);
  }

  t_data_philo *data = NULL;
  // printf("%p\n", &data);
  parse_input(argc, argv, &data);
  if (create_forks(&data))
  {
    free_p(&data);
    return (1);
  }
  if (create_philos(&data))
  {
    free_p(&data);
    return (1);
  }

  for (int i = 0; i < data->n_philo; i++)
  {
    printf("philo id: %d\n", data->philos[i].id);
  }
  init_routine(&data);
    
  free_p(&data);
  return (0);
}
