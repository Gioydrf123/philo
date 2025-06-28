#include "../inc/philo.h"

// un po di orgine extra pls
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
    printf("[%ld] Philosoper %d has taken a fork\n", get_timestamp(&philo_ptr->data->start_time), philo_ptr->id);
    pthread_mutex_lock(&philo_ptr->data->forks[left_fork]);
    printf("[%ld] Philosoper %d has taken a fork\n", get_timestamp(&philo_ptr->data->start_time), philo_ptr->id);
    printf("[%ld] Philosoper %d is eating\n", get_timestamp(&philo_ptr->data->start_time), philo_ptr->id);
	if(philo_ptr->data->tt_die - philo_ptr->data->tt_eat < 0)
	{
		usleep(philo_ptr->data->tt_die * 1000);
        // added print_lock
        pthread_mutex_lock(&philo_ptr->data->print_lock);
        printf("[%ld] Philosoper %d is died\n", get_timestamp(&philo_ptr->data->start_time), philo_ptr->id);
        pthread_mutex_unlock(&philo_ptr->data->print_lock);

		pthread_mutex_lock(&philo_ptr->data->sim);
		philo_ptr->data->simulation_running = false;
		pthread_mutex_unlock(&philo_ptr->data->sim);
	}	
	pthread_mutex_unlock(&philo_ptr->data->forks[right_fork]);
	pthread_mutex_unlock(&philo_ptr->data->forks[left_fork]);
}