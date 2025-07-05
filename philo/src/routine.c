#include "../inc/philo.h"

static void	philo_sleep(t_philo *philo_ptr)
{
	print_philo_lock(philo_ptr, " is sleeping\n");
	usleep(philo_ptr->data->tt_sleep * 1000);
}

static void	swap_int_philo(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	philo_eat(t_philo *philo_ptr)
{
	int	right_fork;
	int	left_fork;

	right_fork = (philo_ptr->id + 1) % philo_ptr->data->n_philo;
	left_fork = philo_ptr->id;
	if ((philo_ptr->id % 2) == 0)
		swap_int_philo(&right_fork, &left_fork);
	pthread_mutex_lock(&philo_ptr->data->forks[right_fork]);
	print_philo_lock(philo_ptr, " has taken a fork\n");
	pthread_mutex_lock(&philo_ptr->data->forks[left_fork]);
	pthread_mutex_lock(&philo_ptr->data->time_lock);
	print_philo(philo_ptr, " has taken a fork\n");
	print_philo(philo_ptr, " is eating\n");
	pthread_mutex_unlock(&philo_ptr->data->time_lock);
	usleep(philo_ptr->data->tt_eat * 1000);
	pthread_mutex_unlock(&philo_ptr->data->forks[right_fork]);
	pthread_mutex_unlock(&philo_ptr->data->forks[left_fork]);
	pthread_mutex_lock(&philo_ptr->data->meal_lock);
	philo_ptr->meal_count--;
	pthread_mutex_unlock(&philo_ptr->data->meal_lock);
	pthread_mutex_lock(&philo_ptr->data->time_lock);
	philo_ptr->last_meal = (int)get_timestamp(&philo_ptr->data->start_time);
	pthread_mutex_unlock(&philo_ptr->data->time_lock);
}

int	simulation_running(t_philo *philo_ptr)
{
	while (philo_ptr->meal_count != 0)
	{
		philo_eat(philo_ptr);
		pthread_mutex_lock(&philo_ptr->data->sim_lock);
		if (philo_ptr->data->simulation_running == false)
			return(pthread_mutex_unlock(&philo_ptr->data->sim_lock), 1);
		pthread_mutex_unlock(&philo_ptr->data->sim_lock);
		philo_sleep(philo_ptr);
		pthread_mutex_lock(&philo_ptr->data->sim_lock);
		if (philo_ptr->data->simulation_running == false)
			return(pthread_mutex_unlock(&philo_ptr->data->sim_lock), 1);
		pthread_mutex_unlock(&philo_ptr->data->sim_lock);
		pthread_mutex_unlock(&philo_ptr->data->sim_lock);
		print_philo_lock(philo_ptr, " is thinking\n");
		pthread_mutex_lock(&philo_ptr->data->sim_lock);
		if (philo_ptr->data->simulation_running == false)
			return(pthread_mutex_unlock(&philo_ptr->data->sim_lock), 1);
		pthread_mutex_unlock(&philo_ptr->data->sim_lock);
	}
	return (0);
}
