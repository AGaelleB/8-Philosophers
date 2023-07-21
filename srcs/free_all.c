/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 19:02:10 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/20 17:23:10 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_all_mutex(t_init *init)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&init->eat_count_mutex);
	pthread_mutex_destroy(&init->death_mutex);
	pthread_mutex_destroy(&init->write_mutex);
	while (i < init->nb_of_philo)
	{
		pthread_mutex_destroy(&init->philo[i].mutex);
		pthread_mutex_destroy(&init->forks[i]);
		i++;
	}
	// free(init->forks);
	// free(init->philo);
	// free(init);
}

void	free_all_forks(t_init *init)
{
	int	i;

	if (init->forks != NULL)
	{
		i = 0;
		while (i < init->nb_of_philo)
		{
			pthread_mutex_destroy(&init->forks[i]);
			i++;
		}
		// free(init->forks);
		init->forks = NULL;
	}
}

void	free_all_data(t_init *init)
{
	int	i;

	i = 0;
	while (i < init->nb_of_philo)
	{
		// free(init->philo[i].data);
		i++;
	}
	// free(init->philo);
}

void	free_all(t_init *data)
{
	free_all_mutex(data);
	free_all_forks(data);
}