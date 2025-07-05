#include "../inc/philo.h"

#include "../inc/philo.h"

static int	ft_isdigit_philo(int c)
{
	if (!(c >= '0' && c <= '9'))
	{
		printf("Error format only digits\n");
		exit (1);
	}
	return (1);
}

static long	ft_atol_philo(const char *str)
{
	long	value;
	int		sign;

	sign = 1;
	value = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	while (ft_isdigit_philo(*str))
	{
		value = value * 10 + (*str++ - '0');
		if (value > 2147483648 && sign == -1)
			return (sign * value);
		if (value > 2147483647 && sign == 1)
			return (sign * value);
	}
	return (sign * value);
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

void	parse_input(int argc, char **argv, t_data_philo **data)
{
	long	check_data[5];

	check_data[0] = ft_atol_philo(argv[1]);
	check_data[1] = ft_atol_philo(argv[2]);
	check_data[2] = ft_atol_philo(argv[3]);
	check_data[3] = ft_atol_philo(argv[4]);
	if (argc == 6)
		check_data[4] = ft_atol_philo(argv[5]);
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
