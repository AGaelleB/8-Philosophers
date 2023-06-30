/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:40:03 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/30 17:31:59 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	write_error(char *str)
{
	int	len;

	len = 0;
	while(str[len])
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
	// 1 sec = 1 000 milisecondes
}

// void	print_action(t_philo *philo, int id, char *str)
// {
// 	pthread_mutex_lock(&philo->init->write_mutex);
// 	printf("%lli ", get_time_philo() - philo->time_init);
// 	printf("%i ", id); // + 1);
// 	printf("%s\n", str);
// 	pthread_mutex_unlock(&philo->init->write_mutex);
// }


void	print_action(t_philo *philo, t_init *init, int id, char *str)
{
	pthread_mutex_lock(&init->write_mutex);
	printf("%lli ", get_time_philo() - philo->time_init);
	printf("%i ", id); // + 1);
	printf("%s\n", str);
	pthread_mutex_unlock(&init->write_mutex);
}


void cleanup_forks(t_init *data)
{
	int	i;

	if (data->forks != NULL)
	{
		i = data->nb_of_philo - 1;
		while (i >= 0)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i--;
		}

		free(data->forks);
		data->forks = NULL;
	}
}


/* FAIRE UNE FONCTION QUI VA SUPP TOUS LES MUTEX*/
/*

destroy the mutex after all threads have finished
pthread_mutex_destroy(&write_mutex);



*/