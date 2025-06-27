#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int id;
    int eat_count;
    int max_eat;
    struct timeval *start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t *print_lock;
    int *time_to_eat;
    int *time_to_sleep;
    int *time_to_think;
    int *time_to_die;
    bool *simulation_running;
} Philosopher;

long get_timestamp(struct timeval *start_time) {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - start_time->tv_sec) * 1000 + 
           (now.tv_usec - start_time->tv_usec) / 1000;
}

void print_status(Philosopher *philo, const char *status) {
    pthread_mutex_lock(philo->print_lock);
    printf("%ld %d %s\n", get_timestamp(philo->start_time), philo->id, status);
    pthread_mutex_unlock(philo->print_lock);
}

void* philosopher_routine(void *arg) {
    Philosopher *philo = (Philosopher*)arg;
    int left_fork = philo->id;
    int right_fork = (philo->id + 1) % 4;
    long last_meal_time = get_timestamp(philo->start_time);

    while (*(philo->simulation_running)) {
        // Take forks (alternate order based on philosopher ID)
        if (philo->id % 2 == 0) {
            pthread_mutex_lock(&philo->forks[left_fork]);
            print_status(philo, "has taken a fork");
            pthread_mutex_lock(&philo->forks[right_fork]);
            print_status(philo, "has taken a fork");
        } else {
            pthread_mutex_lock(&philo->forks[right_fork]);
            print_status(philo, "has taken a fork");
            pthread_mutex_lock(&philo->forks[left_fork]);
            print_status(philo, "has taken a fork");
        }

        // Eating
        print_status(philo, "is eating");
        last_meal_time = get_timestamp(philo->start_time);
        usleep(*(philo->time_to_eat) * 1000);
        philo->eat_count++;

        // Release forks
        pthread_mutex_unlock(&philo->forks[left_fork]);
        pthread_mutex_unlock(&philo->forks[right_fork]);

        // Check if philosopher has eaten enough
        if (philo->max_eat > 0 && philo->eat_count >= philo->max_eat) {
            break;
        }

        // Sleeping
        print_status(philo, "is sleeping");
        usleep(*(philo->time_to_sleep) * 1000);

        // Thinking
        print_status(philo, "is thinking");
        usleep(*(philo->time_to_think) * 1000);

        // Check for death
        if (get_timestamp(philo->start_time) - last_meal_time > *(philo->time_to_die)) {
            print_status(philo, "died");
            *(philo->simulation_running) = false;
            break;
        }
    }
    free(philo);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 5 || argc > 6) {
        printf("Usage: %s <num_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [max_eat_times]\n", argv[0]);
        return 1;
    }

    // Initialize simulation parameters
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    int num_philosophers = atoi(argv[1]);
    int time_to_die = atoi(argv[2]);
    int time_to_eat = atoi(argv[3]);
    int time_to_sleep = atoi(argv[4]);
    int time_to_think = time_to_sleep; // Thinking time same as sleeping time
    int max_eat_times = -1; // Default: eat until someone dies
    
    if (argc == 6) {
        max_eat_times = atoi(argv[5]);
    }

    pthread_mutex_t forks[4];
    pthread_mutex_t print_lock;
    bool simulation_running = true;

    // Initialize mutexes
    for (int i = 0; i < 4; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
    pthread_mutex_init(&print_lock, NULL);

    // Create philosopher threads
    pthread_t philosophers[num_philosophers];
    for (int i = 0; i < num_philosophers; i++) {
        Philosopher *philo = malloc(sizeof(Philosopher));
        philo->id = i;
        philo->eat_count = 0;
        philo->max_eat = max_eat_times;
        philo->start_time = &start_time;
        philo->forks = forks;
        philo->print_lock = &print_lock;
        philo->time_to_eat = &time_to_eat;
        philo->time_to_sleep = &time_to_sleep;
        philo->time_to_think = &time_to_think;
        philo->time_to_die = &time_to_die;
        philo->simulation_running = &simulation_running;
        
        pthread_create(&philosophers[i], NULL, philosopher_routine, philo);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_philosophers; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Cleanup mutexes
    for (int i = 0; i < 4; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    pthread_mutex_destroy(&print_lock);

    return 0;
}
