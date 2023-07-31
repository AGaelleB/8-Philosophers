/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:40:03 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/31 11:19:43 by abonnefo         ###   ########.fr       */
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
	if (check_flag_all_eat(init))
		return ;
	if (check_flag_died(init))
	{
		pthread_mutex_lock(&init->death_printed_mutex);
		if (init->death_printed == 0)
		{
			init->death_printed = 1;
			pthread_mutex_unlock(&init->death_printed_mutex);
			pthread_mutex_lock(&init->write_mutex);
			printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
			printf("died\n");
			pthread_mutex_unlock(&init->write_mutex);
		}
		else
			pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
	pthread_mutex_lock(&init->write_mutex);
	printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
	printf("%s\n", str);
	pthread_mutex_unlock(&init->write_mutex);
}
