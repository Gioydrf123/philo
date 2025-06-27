#include "../inc/philo.h"


// long get_timestamp(struct timeval *t/ime_start)
// {
//   struct timeval now;
//   gettimeofday(&now, NULL);
//   return (now.tv_sec - start_time->tv_sec) * 1000 + (now.tv_usec - start_time->tv_usec) / 1000;
// }

int  free_p(t_data_philo **data, short ret)
{
  if ((*data)->forks)
  {
    int i;
    i = (*data)->n_philo;
    while(i-- > 0)
      sem_destroy(&(*data)->forks);
  }
  // free((*data)->forks);
  if ((*data)->philos)
    free((*data)->philos);
  if ((*data)->ids)
    free((*data)->ids);
  if ((*data))
    free((*data));
  return (ret);
}

int create_sem_forks(t_data_philo **data)
{
  int i;
  i = 0;

  // semaphores have an integer inside their structure

  while (i < (*data)->n_philo)
  {
    // initializing array of semaphores
    // TODO: add control for failing
    sem_init(&(*data)->forks[i], 0, 1);
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

// sem_post : like mutex unlock

void philo_eat(t_philo *philo_ptr)
{
    if ((data->id % 2) == 0)
     {
      sem_wait((*data)->forks[right_fork]);
      sem_wait((*data)->fors[left_fork]);
        //pthread_mutex_lock(&philo->data->forks[right_fork]);
        //printf("Philosoper %d has taken a fork\n",data->id);
        //pthread_mutex_lock(&philo->data->forks[left_fork]);
        //printf("Philosoper %d has taken a fork\n", data->id);
     }
     else
     {
      sem_wait((*data)->forks[left_fork]);
      sem_wait((*data)->fors[right_fork]);
      // pthread_mutex_lock(&philo->data->forks[left_fork]);
      // printf("Philosoper %d has taken a fork\n", data->id);
      // pthread_mutex_lock(&philo->data->forks[right_fork]);
      // printf("Philosoper %d has taken a fork\n",data->id);
     }
     printf("Philosoper %d is eating\n",data->id);
}

void simulation_running(t_philo *philo_ptr)
{
  bool simulation_running = true;

  while(simulation_running)
  {
    // philo_eat(philo_ptr);
    /*ATTENZIONE FUNZIONI DA IMPLEMENTARE LAVORARE SU QUESTE FUNZIONI SE POSSIBILE,
    SI CAPISCE COSA FANNO 
    philo_sleep();
    philo_think();*/
    i++;
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


  printf("filosofo id :%d\n", data->id);
  simulation_running(philo);
  printf("hello world");
}

int init_routine(t_data_philo **data)
{
  int i;
  i = 0;
  while (i < (*data)->n_philo)
  {
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
    return (printf("error\n"));
  t_data_philo *data = NULL;
  parse_input(argc, argv, &data);
  // updated create fork with semaphores
  if (create_sem_forks(&data))
    return (free_p(&data, 1));

  ///////////////////////////////
  if (create_philos(&data))
    return (free_p(&data, 1));
  // printf("original data address: %p\n", data);
  init_routine(&data);

  free_p(&data, 0);
  return (0);
}
