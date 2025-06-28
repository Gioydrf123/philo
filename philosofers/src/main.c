#include "../inc/philo.h"


long get_timestamp(struct timeval *start_time)
{
  struct timeval now;
  gettimeofday(&now, NULL);
  return (now.tv_sec - start_time->tv_sec)* 1000 + (now.tv_usec - start_time->tv_usec)/ 1000;
}

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
  (*data)->simulation_running = true;
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

void	swap_int_philo(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a=*b;
	*b = tmp;
}

void philo_eat(t_philo *philo_ptr)
{
	//da implementare un mutex per impedire race condition su stampa
	int right_fork = (philo_ptr->id + 1) % philo_ptr->data->n_philo;
	int left_fork = philo_ptr->id;

//	printf("tempo teorico 1: %d\n",philo_ptr->data->tt_die);
//	printf("tempo teorico 2: %d\n",philo_ptr->data->tt_eat);
	if ((philo_ptr->id % 2) == 0)
		swap_int_philo(&right_fork, &left_fork);
	pthread_mutex_lock(&philo_ptr->data->forks[right_fork]);
//	printf("[%ld] Philosoper %d has taken a fork\n", get_timestamp(&philo_ptr->data->start_time), philo_ptr->id);
	pthread_mutex_lock(&philo_ptr->data->forks[left_fork]);
//	printf("[%ld] Philosoper %d has taken a fork\n", get_timestamp(&philo_ptr->data->start_time), philo_ptr->id);
//	printf("[%ld] Philosoper %d is eating\n", get_timestamp(&philo_ptr->data->start_time), philo_ptr->id);
	if(philo_ptr->data->tt_die - philo_ptr->data->tt_eat < 0)
	{
		usleep(philo_ptr->data->tt_die * 1000);
//		printf("[%ld] Philosoper %d is died\n", get_timestamp(&philo_ptr->data->start_time), philo_ptr->id);
		pthread_mutex_lock(&philo_ptr->data->sim);
		philo_ptr->data->simulation_running = false;
		pthread_mutex_unlock(&philo_ptr->data->sim);
	}	
	pthread_mutex_unlock(&philo_ptr->data->forks[right_fork]);
	pthread_mutex_unlock(&philo_ptr->data->forks[left_fork]);
}

void simulation_running(t_philo *philo_ptr)
{
  while(1)
  {
    philo_eat(philo_ptr);
	
	pthread_mutex_lock(&philo_ptr->data->sim);
	if (philo_ptr->data->simulation_running == false)
		break ;
	pthread_mutex_unlock(&philo_ptr->data->sim);
    /*ATTENZIONE FUNZIONI DA IMPLEMENTARE LAVORARE SU QUESTE FUNZIONI SE POSSIBILE,
    SI CAPISCE COSA FANNO 
    philo_sleep();
    philo_think();*/
  }
	pthread_mutex_unlock(&philo_ptr->data->sim);
}

void *philosophers_routine(void *philo_ptr)
{
  t_philo *philo;
  philo = (t_philo *)philo_ptr;
  simulation_running(philo);
  return (NULL);
}

int init_routine(t_data_philo **data)
{
  int i;
  i = 0;
  while (i < (*data)->n_philo)
  {
    (*data)->philos[i].id = i;
    gettimeofday(&(*data)->start_time, NULL);
    if (pthread_create(&(*data)->ids[i], NULL, philosophers_routine, &(*data)->philos[i]))
      return printf("error in thread creation\n");
    i++;
  }
  while (i-- > 0)
		pthread_join((*data)->ids[i], NULL);
	return (0);
}

int main(int argc, char **argv)
{
  if (argc < 4 || argc > 5)
  {
    printf("error\n");
    return (1);
  }

  t_data_philo *data = NULL;
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
