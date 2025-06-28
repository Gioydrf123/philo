#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct t_name_philo
{
	int id;
 
	// struct timeval *start_time;
	// pthread_mutex_t *fork_left;
	// pthread_mutex_t *fork_right;
	struct t_name_data_philo *data;
}	t_philo;

typedef struct t_name_data_philo
{
	int n_philo;
	int tt_eat;
	int tt_sleep;
	int tt_die;
	int max_eat;
	struct timeval	start_time;
	t_philo *philos;
	pthread_t *ids;
	pthread_mutex_t *forks;
	//per stampa
	pthread_mutex_t time_lock;
	pthread_mutex_t print_lock;
	pthread_mutex_t sim;
	bool simulation_running;
}	t_data_philo;

// Function Prototypes
int	init_routine(t_data_philo **data);
int	free_p(t_data_philo **data, short ret);
void parse_input(int argc, char **argv, t_data_philo **data);
void philo_eat(t_philo *philo_ptr);
long get_timestamp(struct timeval *start_time);
void	swap_int_philo(int *a, int *b);
