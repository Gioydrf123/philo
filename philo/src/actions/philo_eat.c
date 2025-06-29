#include "../../inc/philo.h"


void    ft_putnbr(int n)
{
    char    buffer[12]; // Enough to hold the largest 32-bit int (2147483647)
    int     i = 0;

    if (n == 0)
    {
        write(1, "0", 1);
        return;
    }

    // Extract digits and store them in reverse order
    while (n > 0)
    {
        buffer[i++] = (n % 10) + '0'; // Convert digit to ASCII
        n /= 10;
    }

    // Write the digits in the correct order (reverse of how they were stored)
    while (i > 0)
    {
        write(1, &buffer[--i], 1);
    }
}

void	ft_putstr(char *s)
{
	while (*s)
	{
		write(1, s, 1);
		s++;
	}
}

void print_philo_lock(t_philo *philo_ptr, char *s)
{
	pthread_mutex_lock(&philo_ptr->data->time_lock);
	write(1, "[", 1);
	ft_putnbr((int)get_timestamp(&philo_ptr->data->start_time));
	write(1, "]", 1);
	write(1, " Philosofer " ,12);
	ft_putnbr(philo_ptr->id);
	ft_putstr(s);
	pthread_mutex_unlock(&philo_ptr->data->time_lock);
}

void print_philo(t_philo *philo_ptr, char *s)
{
	write(1, "[", 1);
	ft_putnbr((int)get_timestamp(&philo_ptr->data->start_time));
	write(1, "]", 1);
	write(1, " Philosofer " ,12);
	ft_putnbr(philo_ptr->id);
	ft_putstr(s);
}

void sleep_ms(long ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void philo_eat(t_philo *philo_ptr)
{
	int right_fork = (philo_ptr->id + 1) % philo_ptr->data->n_philo;
	int left_fork = philo_ptr->id;

	if ((philo_ptr->id % 2) == 0)
		swap_int_philo(&right_fork, &left_fork);
	pthread_mutex_lock(&philo_ptr->data->forks[right_fork]);
	print_philo_lock(philo_ptr, " has taken a fork\n");
	pthread_mutex_lock(&philo_ptr->data->forks[left_fork]);
	pthread_mutex_lock(&philo_ptr->data->time_lock);
	print_philo(philo_ptr, " has taken a fork\n");
	print_philo(philo_ptr, " is eating\n");
	pthread_mutex_unlock(&philo_ptr->data->time_lock);
//	print_philo_lock(philo_ptr, " timestamp\n");
	if(philo_ptr->data->tt_die - philo_ptr->data->tt_eat < 0)
	{
//		print_philo_lock(philo_ptr, " timestamp\n");
		usleep(philo_ptr->data->tt_die * 1000);
		print_philo_lock(philo_ptr, " is died1\n");
		pthread_mutex_lock(&philo_ptr->data->sim_lock);
		philo_ptr->data->simulation_running = false;
		pthread_mutex_unlock(&philo_ptr->data->sim_lock);
	}
	usleep(philo_ptr->data->tt_eat * 1000);
	pthread_mutex_unlock(&philo_ptr->data->forks[right_fork]);
	pthread_mutex_unlock(&philo_ptr->data->forks[left_fork]);
	philo_ptr->meal_count++;
}
