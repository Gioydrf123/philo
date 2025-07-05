/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:58:05 by gmu               #+#    #+#             */
/*   Updated: 2025/07/05 12:36:13 by gmu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct t_name_philo
{
	int							id;
	int							meal_count;
	int							last_meal;
	// struct timeval *start_time;
	// pthread_mutex_t *fork_left;
	// pthread_mutex_t *fork_right;
	struct t_name_data_philo	*data;
}								t_philo;

typedef struct t_name_data_philo
{
	int							n_philo;
	int							tt_eat;
	int							tt_sleep;
	int							tt_die;
	int							max_eat;
	struct timeval				start_time;
	t_philo						*philos;
	pthread_t					*ids;
	pthread_mutex_t				*forks;
	// per stampa
	pthread_mutex_t				meal_lock;
	pthread_mutex_t				time_lock;
	pthread_mutex_t				print_lock;
	pthread_mutex_t				sim_lock;
	bool						simulation_running;

	pthread_mutex_t				*philo_locks;// Mutex per ogni filosofo
	pthread_t					monitor;// Thread del monitor
}								t_data_philo;

// Function Prototypes
int								free_p(t_data_philo **data, short ret);
//
void							parse_input(int argc, char **argv, t_data_philo **data);
//
void							*monitor_routine(void *data_ptr);
//
int							simulation_running(t_philo *philo_ptr);
void							print_philo_lock(t_philo *philo_ptr, char *s);
void							print_philo(t_philo *philo_ptr, char *s);
long							get_timestamp(struct timeval *start_time);
int								free_p(t_data_philo **data, short ret);
