#include "../inc/philo.h"

long	get_timestamp(struct timeval *start_time)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start_time->tv_sec) * 1000 + (now.tv_usec
			- start_time->tv_usec) / 1000);
}
