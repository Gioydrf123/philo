#include "../inc/philo.h"


// long get_timestamp(struct timeval *t/ime_start)
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
  for(int i = 0;i<(*data)->n_philo; ++i)
  {
    printf("%p\n",&(*data)->forks[i]);
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
  //array di filosofi dentro data
  (*data)->philos = calloc(sizeof(t_philo), (*data)->n_philo);
  //ids sono gli ids
  (*data)->ids = calloc(sizeof(pthread_t), (*data)->n_philo);
  if (!(*data)->philos || !(*data)->ids)
  {
    return (true);
  }
  while(i < (*data)->n_philo)
  {
    (*data)->philos[i].id = i;
    (*data)->philos[i].data = (*data); 
    i++;
  }
  return (false);
}
/*
void philo_eat(t_philo *philo_ptr)
{
    if ((data->id % 2) == 0)
     {
      pthread_mutex_lock(&philo->data->forks[right_fork]);
      printf("Philosoper %d has taken a fork\n",data->id);
      pthread_mutex_lock(&philo->data->forks[left_fork]);
      printf("Philosoper %d has taken a fork\n", data->id);
     }
     else
     {
      pthread_mutex_lock(&philo->data->forks[left_fork]);
      printf("Philosoper %d has taken a fork\n", data->id);
      pthread_mutex_lock(&philo->data->forks[right_fork]);
      printf("Philosoper %d has taken a fork\n",data->id);
     }
     printf("Philosoper %d is eating\n",data->id);
}*/
void simulation_running(t_philo *philo_ptr)
{
  bool simulation_running = true;

  while(simulation_running)
  {
    //philo_eat(philo_ptr);
    /*ATTENZIONE FUNZIONI DA IMPLEMENTARE LAVORARE SU QUESTE FUNZIONI SE POSSIBILE,
    SI CAPISCE COSA FANNO 
    philo_sleep();
    philo_think();*/
  }
}

void *philosophers_routine(void *philo_ptr)
{
  t_philo *philo;
  philo = (t_philo *)philo_ptr;
  int left_fork = philo->id;
  int right_fork = (philo->id + 1) % philo->data->n_philo;
  // bool simulation_running= true;

  printf("philo id: %d\n", philo->id);
  printf("philo id: %d left fork: %d\n",philo->id, left_fork);
  printf("philo id: %d pointer left fork: %p\n",philo->id, &philo->data->forks[left_fork]);
  printf("philo id: %d right fork: %d\n",philo->id, right_fork);
  printf("philo id: %d pointer right fork: %p\n",philo->id, &philo->data->forks[right_fork]);
  printf("philo id: %d data address = %p\n", philo->id, &philo->data);


//  printf("filosofo id :%d\n", data->id);
  simulation_running(philo);
  printf("hello world");
}

int init_routine(t_data_philo **data)
{
  int i;
  i = 0;
  while (i < (*data)->n_philo)
  {
    (*data)->id = i;
    if (pthread_create(&(*data)->ids[i], NULL, &philosophers_routine, &(*data)->philos[i]))
      return printf("error in thread creation\n");
    usleep(1);
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
  printf("original data address: %p\n", data);
  init_routine(&data);

  free_p(&data);
  return (0);
}
