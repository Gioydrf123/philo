#include "../inc/philo.h"

void	swap_int_philo(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a=*b;
	*b = tmp;
}

long get_timestamp(struct timeval *start_time)
{
  struct timeval now;
  gettimeofday(&now, NULL);
  return (now.tv_sec - start_time->tv_sec)* 1000 + (now.tv_usec - start_time->tv_usec)/ 1000;
}

int  free_p(t_data_philo **data, short ret)
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