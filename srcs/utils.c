/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:40:03 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/26 18:03:36 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	write_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(2, "Error: ", 7);
	write(2, str, len);
	write(2, "\n", 1);
	return (0);
}

long long	get_time_philo(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_action(t_init *init, int id, char *str)
{
	if (init->end_flag != 1)
	{
		pthread_mutex_lock(&init->write_mutex);
		printf("%lld ", (get_time_philo() - init->philo->time_init));
		printf("%d ", id);
		printf("%s\n", str);
		pthread_mutex_unlock(&init->write_mutex);
	}
}
