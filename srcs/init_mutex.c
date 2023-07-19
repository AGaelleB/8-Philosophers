/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/19 19:11:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_init	*init_mutex(t_init *data)
{
	int	i;
	int	mutex;

	i = data->nb_of_philo - 1;
	while (i >= 0)
	{
		mutex = pthread_mutex_init(&data->philo[i].mutex, NULL);
		if (mutex != 0)
		{
			printf("Failed to initialize mutex for philosopher %d\n", i);
			free(data->philo);
			free(data);
			// cleanup_all_mutex(data); // ADD
			return (NULL);
		}
		i--;
	}
	return (data);
}

t_init	*init_write_mutex(t_init *init)
{
	if (pthread_mutex_init(&init->write_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for write\n");
		free(init);
		// cleanup_all_mutex(init); // ADD
		return (NULL);
	}
	return (init);
}

t_init	*init_eat_count_mutex(t_init *data)
{
	if (pthread_mutex_init(&data->eat_count_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for eat count\n");
		free(data->philo);
		free(data);
		// cleanup_all_mutex(data); // ADD
		return (NULL);
	}
	return (data);
}

t_init	*init_death_mutex(t_init *data)
{
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for death check\n");
		free(data->philo);
		free(data);
		// cleanup_all_mutex(data); // ADD
		return (NULL);
	}
	return (data);
}

t_init	*init_forks(t_init *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_of_philo);
	if (data->forks == NULL)
		return (NULL);
	i = data->nb_of_philo - 1;
	while (i >= 0)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (i < data->nb_of_philo)
			{
				pthread_mutex_destroy(&data->forks[i]);
				i++;
			}
			free(data->forks);
			return (NULL);
		}
		i--;
	}
	return (data);
}
