#include "../inc/philo.h"

static void simulation_running(t_philo *philo_ptr)
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

static void *philosophers_routine(void *philo_ptr)
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
    // changed non protected access to start_time
    pthread_mutex_lock(&(*data)->time_lock);
    gettimeofday(&(*data)->start_time, NULL);
    pthread_mutex_unlock(&(*data)->time_lock);

    if (pthread_create(&(*data)->ids[i], NULL, philosophers_routine, &(*data)->philos[i]))
      return printf("error in thread creation\n");
    // usleep(1);
    i++;
  }
  while (i-- > 0)
		pthread_join((*data)->ids[i], NULL);
	return (0);
}