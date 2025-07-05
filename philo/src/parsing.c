#include "../inc/philo.h"

static int	ft_isdigit_philo(int c)
{
	if (!(c >= '0' && c <= '9'))
		return (0);
	return (1);
}

static long	ft_atol_philo(const char *str)
{
	long	value;

	value = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	while (*str)
	{
		if (!ft_isdigit_philo(*str))
		{
			printf("Error - only digits allowed\n");
			exit (1);
		}
		value = value * 10 + (*str - '0');
		if (value > 2147483647)
			return (value);
		str++;
	}
	return (value);
}

static int	ft_atoi_philo(const char *str)
{
	int	sign;
	int	value;

	sign = 1;
	value = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	while (ft_isdigit_philo(*str))
		value = value * 10 + (*str++ - '0');
	return (sign * value);
}

bool	check_min_max(int argc, char **argv)
{
	long	check_data[5];
	int		i;

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

void	parse_input(int argc, char **argv, t_data_philo **data)
{
	if (check_min_max(argc, argv))
		exit (1);
	*data = calloc(sizeof(t_data_philo), 1);
	(*data)->n_philo = ft_atoi_philo(argv[1]);
	(*data)->tt_die = ft_atoi_philo(argv[2]);
	(*data)->tt_eat = ft_atoi_philo(argv[3]);
	(*data)->tt_sleep = ft_atoi_philo(argv[4]);
	if (argc == 6)
		(*data)->max_eat = ft_atoi_philo(argv[5]);
	else
		(*data)->max_eat = -1;
	printf("numero filosofi : %d\n", (*data)->n_philo);
	printf("time to die : %d\n", (*data)->tt_die);
	printf("time to eat : %d\n", (*data)->tt_eat);
	printf("time to sleep : %d\n", (*data)->tt_sleep);
	(*data)->simulation_running = true;
}
