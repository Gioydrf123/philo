#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_philo
{
    int        id;
    int        n_philo;
    int        tt_die;
    int        tt_eat;
    int        tt_sleep;
    int        max_eat;
    long    start_time;
    sem_t    *forks_sem;
    sem_t    *room_sem;
    sem_t    *output_sem;
    sem_t    *death_sem;
}            t_philo;

long    get_time_in_ms(void)
{
    struct timeval    tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void    print_action(t_philo *philo, const char *msg)
{
    long    current_time;

    sem_wait(philo->output_sem);
    current_time = get_time_in_ms() - philo->start_time;
    printf("%ld %d %s\n", current_time, philo->id, msg);
    sem_post(philo->output_sem);
}

void    philo_exit(t_philo *philo)
{
    sem_wait(philo->death_sem);
    sem_wait(philo->output_sem);
    printf("%ld %d died\n", get_time_in_ms() - philo->start_time, philo->id);
    exit(1);
}

void    philo_release(t_philo *philo)
{
    sem_post(philo->forks_sem);
    sem_post(philo->forks_sem);
    sem_post(philo->room_sem);
}

void    philosopher(t_philo *philo)
{
    long    last_eat;
    int        meals;

    last_eat = get_time_in_ms();
    meals = 0;
    while (1)
    {
        if (get_time_in_ms() - last_eat > philo->tt_die)
            philo_exit(philo);
        print_action(philo, "is thinking");
        sem_wait(philo->room_sem);
        sem_wait(philo->forks_sem);
        print_action(philo, "has taken a fork");
        if (philo->n_philo == 1)
        {
            usleep(philo->tt_die * 1000);
            sem_post(philo->forks_sem);
            sem_post(philo->room_sem);
            philo_exit(philo);
        }
        sem_wait(philo->forks_sem);
        print_action(philo, "has taken a fork");
        last_eat = get_time_in_ms();
        meals++;
        print_action(philo, "is eating");
        usleep(philo->tt_eat * 1000);
        philo_release(philo);
        if (philo->max_eat > 0 && meals >= philo->max_eat)
            exit(0);
        print_action(philo, "is sleeping");
        usleep(philo->tt_sleep * 1000);
    }
}

t_philo    initialize_philo(int id, long *data, sem_t *array[4])
{
    t_philo    philo = {.id = id, .n_philo = (int)data[0], .tt_die = (int)data[1],
            .tt_eat = (int)data[2], .tt_sleep = (int)data[3],
            .max_eat = (int)data[4], .start_time = data[5],
            .forks_sem = array[0], .room_sem = array[1], .output_sem = array[2],
            .death_sem = array[3]};

    return (philo);
}

int    create_philosophers(long *data, sem_t *array[4], pid_t pids[])
{
    pid_t    pid;
    t_philo    philo;
    int i;

    i = 0;
    while (i < data[0])
    {
        pid = fork();
        if (pid == 0)
        {
            philo = initialize_philo(i, data, array);
            philosopher(&philo);
            exit(0);
        }
        else if (pid > 0)
            pids[i] = pid;
        else
        {
            perror("fork failed");
            return (1);
        }
        i++;
    }
    return (0);
}

void    sem_un(void)
{
    sem_unlink("/forks");
    sem_unlink("/room");
    sem_unlink("/output");
    sem_unlink("/death");
}

static int    ft_isdigit_philo(int c)
{
    if (!(c >= '0' && c <= '9'))
        return (0);
    return (1);
}

static long    ft_atol_philo(const char *str)
{
    long    value;

    value = 0;
    while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
        str++;
    while (*str)
    {
        if (!ft_isdigit_philo(*str))
        {
            printf("Error - only digits allowed\n");
            exit(1);
        }
        value = value * 10 + (*str - '0');
        if (value > 2147483647)
            return (value);
        str++;
    }
    return (value);
}

bool    check_min_max(int argc, char **argv)
{
    long    check_data[5];
    int        i;

    i = 0;
    check_data[0] = ft_atol_philo(argv[1]);
    check_data[1] = ft_atol_philo(argv[2]);
    check_data[2] = ft_atol_philo(argv[3]);
    check_data[3] = ft_atol_philo(argv[4]);
    while (i < 4)
        if (check_data[i] <= 0 || check_data[i++] > 2147483648)
            return (printf("Error - max int or negative int\n"), true);
    if (argc == 6)
    {
        check_data[4] = ft_atol_philo(argv[5]);
        if (check_data[4] <= 0 || check_data[4] > 2147483648)
            return (printf("Error - max in or negative int\n"), true);
    }
    return (false);
}

void    parse_input(long *data, int argc, char **argv)
{
    if (check_min_max(argc, argv))
        exit(1);
    data[0] = ft_atol_philo(argv[1]);
    data[1] = ft_atol_philo(argv[2]);
    data[2] = ft_atol_philo(argv[3]);
    data[3] = ft_atol_philo(argv[4]);
    data[4] = -1;
    if (argc == 6)
        data[4] = ft_atol_philo(argv[5]);
}

void    sem_op(sem_t *array[4], long data[6])
{
    // Handle room_sem for single philosopher
    long room_sem_value = (data[0] == 1) ? 1 : data[0] - 1;
    
    array[0] = sem_open("/forks", O_CREAT, 0666, data[0]);
    array[1] = sem_open("/room", O_CREAT, 0666, room_sem_value);
    array[2] = sem_open("/output", O_CREAT, 0666, 1);
    array[3] = sem_open("/death", O_CREAT, 0666, 1);
    if (array[0] == SEM_FAILED || array[1] == SEM_FAILED
        || array[2] == SEM_FAILED || array[3] == SEM_FAILED)
    {
        printf("sem_open failed\n");
        exit(1);
    }
}

void    sem_cl(sem_t *array[4])
{
    sem_close(array[0]);
    sem_close(array[1]);
    sem_close(array[2]);
    sem_close(array[3]);
}

void    term(long *data, pid_t *pids)
{
    int    status;
    int    i;
    int    j;

    i = 0;
    while (i < (int)data[0])
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
        {
            j = 0;
            while (j < (int)data[0])
            {
                if (j != i)
                    kill(pids[j], SIGKILL);
                j++;
            }
            return ;
        }
        i++;
    }
}

static void    ft_bzero_variant(void *s, size_t n)
{
    char    *p;

    p = s;
    while (n--)
        *p++ = '\0';
}

void    *ft_calloc_variant(size_t count, size_t size)
{
    void    *ptr;

    ptr = malloc(size * count);
    if (!ptr)
    {
        write(1, "Memory allocation error\n", 25);
        free(ptr);
        exit(1);
    }
    ft_bzero_variant(ptr, (count * size));
    return (ptr);
}

int    main(int argc, char **argv)
{
    long    data[6];
    sem_t    *array[4];
    pid_t    *pids;

    if (argc < 5 || argc > 6)
    {
        printf("Usage: %s number_of_philosophers", argv[0]);
        return (printf(" time_to_die time_to_eat time_to_sleep [must_eat_times]\n"));
    }
    sem_un();
    parse_input(data, argc, argv);
    sem_op(array, data);
    data[5] = get_time_in_ms();
    pids = ft_calloc_variant(sizeof(pid_t), data[0]);
    if (!pids)
    {
        printf("calloc error\n");
        exit(1);
    }
    if (create_philosophers(data, array, pids) != 0)
        return (1);
    term(data, pids);
    sem_cl(array);
    sem_un();
    free(pids);
    return (0);
}
