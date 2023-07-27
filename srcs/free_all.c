/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 19:02:10 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/27 19:46:41 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_data(t_data *data)
{
	free(data);
}

void free_all_mutex(t_init *init)
{
	int i = 0;
	while (i < init->nb_of_philo)
	{
		pthread_mutex_destroy(&init->philo[i].eat_mutex);
		i++;
	}
	pthread_mutex_destroy(&init->write_mutex);
	pthread_mutex_destroy(&init->flag_died_mutex);
	pthread_mutex_destroy(&init->flag_all_eat_mutex);
	pthread_mutex_destroy(&init->death_printed_mutex);
}

void	free_all_forks(t_init *init)
{
	int	i;

	i = 0;
	while (i < init->nb_of_philo)
	{
		pthread_mutex_destroy(&init->forks[i]);
		i++;
	}
}

void free_all_mutex_and_forks(t_init *init)
{
	free_all_forks(init);
	free_all_mutex(init);
	free(init->forks);
	free(init->philo);
	free(init);
}
