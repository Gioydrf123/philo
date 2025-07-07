#include "../inc/philo.h"

static void	*philosophers_routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	simulation_running(philo);
	return (NULL);
}

static int	init_routine(t_data_philo **data)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	pthread_create(&monitor, NULL, monitor_routine, *data);
	while (i < (*data)->n_philo)
	{
		if ((*data)->n_philo % 2 == 1)
			usleep(100);
		(*data)->philos[i].id = i;
		pthread_mutex_lock(&(*data)->time_lock);
		gettimeofday(&(*data)->start_time, NULL);
		pthread_mutex_unlock(&(*data)->time_lock);
		if (pthread_create(&(*data)->ids[i], NULL, philosophers_routine,
				&(*data)->philos[i]))
			return (printf("error in thread creation\n"));
		i++;
	}
	pthread_join((*data)->ids[--i], NULL);
	while (i-- > 0)
		pthread_join((*data)->ids[i], NULL);
	pthread_join(monitor, NULL);
	return (0);
}

static int	create_forks(t_data_philo **data)
{
	int	i;

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

static bool	create_philos(t_data_philo **data)
{
	int	i;

	i = 0;
	(*data)->philos = calloc(sizeof(t_philo), (*data)->n_philo);
	(*data)->ids = calloc(sizeof(pthread_t), (*data)->n_philo);
	if (!(*data)->philos || !(*data)->ids)
		return (true);
	while (i < (*data)->n_philo)
	{
		(*data)->philos[i].id = i;
		(*data)->philos[i].data = (*data);
		(*data)->philos[i].meal_count = (*data)->max_eat;
		(*data)->philos[i].last_meal = 0;
		i++;
	}
	return (false);
}

int	main(int argc, char **argv)
{
	t_data_philo	*data;

	if (argc < 4 || argc > 6)
		return (printf("arg error\n"), 1);
	data = NULL;
	parse_input(argc, argv, &data);
	if (create_forks(&data))
		return (free_p(&data, 1));
	if (create_philos(&data))
		return (free_p(&data, 1));
	init_routine(&data);
	free_p(&data, 0);
	return (0);
}
