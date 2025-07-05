#include "../inc/philo.h"

static void	ft_putnbr(int n)
{
	int	i;

	char buffer[12]; // Enough to hold the largest 32-bit int (2147483647)
	i = 0;
	if (n == 0)
	{
		write(1, "0", 1);
		return ;
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

static void	ft_putstr(char *s)
{
	while (*s)
	{
		write(1, s, 1);
		s++;
	}
}

void	print_philo_lock(t_philo *philo_ptr, char *s)
{
	pthread_mutex_lock(&philo_ptr->data->time_lock);
	ft_putnbr((int)get_timestamp(&philo_ptr->data->start_time));
	write(1, " ", 1);
	ft_putnbr(philo_ptr->id);
	ft_putstr(s);
	pthread_mutex_unlock(&philo_ptr->data->time_lock);
}

void	print_philo(t_philo *philo_ptr, char *s)
{
	ft_putnbr((int)get_timestamp(&philo_ptr->data->start_time));
	write(1, " ", 1);
	ft_putnbr(philo_ptr->id);
	ft_putstr(s);
}
