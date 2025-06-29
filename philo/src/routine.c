#include "../inc/philo.h"

static void philo_sleep(t_philo *philo_ptr)
{
//PER IL MOMENTO E' COSI' BISOGNA SOSTITUIRE CON I WRITE NEI PRINTF
//  printf("tt_sleep : %d\n", philo_ptr->data->tt_sleep);
//  printf("tt_die : %d\n", philo_ptr->data->tt_die);
//  printf("controllo : %d\n", philo_ptr->data->tt_sleep - philo_ptr->data->tt_die);
	if ((philo_ptr->data->tt_sleep - philo_ptr->data->tt_die) > 0)
	{
 		print_philo_lock(philo_ptr, " is sleeping1\n");
		usleep(philo_ptr->data->tt_die * 1000);
 		print_philo_lock(philo_ptr, " is died\n");
 		pthread_mutex_lock(&philo_ptr->data->sim_lock);
		philo_ptr->data->simulation_running = false;
		pthread_mutex_unlock(&philo_ptr->data->sim_lock);

	}
	else
	{
		print_philo_lock(philo_ptr, " is sleeping2\n");
		usleep(philo_ptr->data->tt_sleep * 1000);
	}
}


static void philo_think(t_philo *philo_ptr)
{
	print_philo_lock(philo_ptr, " is thinking\n");
	if ((philo_ptr->data->tt_die - philo_ptr->data->tt_eat - philo_ptr->data->tt_sleep) < 0)
	{
		philo_eat(philo_ptr);
		if ((philo_ptr->data->tt_die - philo_ptr->data->tt_eat) < 0)
			return ;
 		print_philo_lock(philo_ptr, " is sleeping1\n");
		usleep((philo_ptr->data->tt_die - philo_ptr->data->tt_eat) * 1000);
 		print_philo_lock(philo_ptr, " is died\n");
 		pthread_mutex_lock(&philo_ptr->data->sim_lock);
		philo_ptr->data->simulation_running = false;
		pthread_mutex_unlock(&philo_ptr->data->sim_lock);
	}	
}

static void simulation_running(t_philo *philo_ptr)
{
  while(1)
  {
    philo_eat(philo_ptr);
    // added exiting condition for max_meals
    pthread_mutex_lock(&philo_ptr->data->meal_lock);
    if (philo_ptr->data->max_eat > 0 && philo_ptr->meal_count >= philo_ptr->data->max_eat)
    {
      pthread_mutex_unlock(&philo_ptr->data->meal_lock);
      break ;
    }
    pthread_mutex_unlock(&philo_ptr->data->meal_lock);






    pthread_mutex_lock(&philo_ptr->data->sim_lock);
    if (philo_ptr->data->simulation_running == false)
    {
      pthread_mutex_unlock(&philo_ptr->data->sim_lock);
      break ;
    }
    pthread_mutex_unlock(&philo_ptr->data->sim_lock);
    //i due blocchi di mutex sopra sono da comprimere
    
    
    
    
    philo_sleep(philo_ptr);




    pthread_mutex_lock(&philo_ptr->data->sim_lock);
    if (philo_ptr->data->simulation_running == false)
    {
      pthread_mutex_unlock(&philo_ptr->data->sim_lock);
      break ;
    }
    pthread_mutex_unlock(&philo_ptr->data->sim_lock);





    philo_think(philo_ptr);





    pthread_mutex_lock(&philo_ptr->data->sim_lock);
    if (philo_ptr->data->simulation_running == false)
    {
      pthread_mutex_unlock(&philo_ptr->data->sim_lock);
      break ;
    }
    pthread_mutex_unlock(&philo_ptr->data->sim_lock);
  }
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
    i++;
  }
  while (i-- > 0)
		pthread_join((*data)->ids[i], NULL);
	return (0);
}
