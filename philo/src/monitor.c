#include "../inc/philo.h"

void	*monitor_routine(void *data_ptr)
{
	t_data_philo	*data;
	int			current_time;
	int			i;

	data = (t_data_philo *)data_ptr;
	while (1)
	{
		i = 0;
		while (i < data->n_philo)
		{
			pthread_mutex_lock(&data->time_lock);
			current_time = (int)get_timestamp(&data->start_time);
			if ((data->philos[i].last_meal + data->tt_die) < current_time)
			{
				printf("%d %d died\n", current_time, i + 1);
				pthread_mutex_unlock(&data->time_lock);
				pthread_mutex_lock(&data->sim_lock);
				data->simulation_running = false;
				pthread_mutex_unlock(&data->sim_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->time_lock);
			++i;
		}
		usleep(1000);
	}
}
