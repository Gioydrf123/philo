#include "../inc/philo.h"

int	free_p(t_data_philo **data, short ret)
{
	if ((*data)->forks)
		free((*data)->forks);
	if ((*data)->philos)
		free((*data)->philos);
	if ((*data)->ids)
		free((*data)->ids);
	if ((*data))
		free((*data));
	return (ret);
}
