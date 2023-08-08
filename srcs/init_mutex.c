/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/08 11:17:23 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_init	*init_write_mutex(t_init *init)
{
	if (pthread_mutex_init(&init->write_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for write\n");
		free(init);
		return (NULL);
	}
	return (init);
}

t_init	*init_flag_died_mutex(t_init *init)
{
	if (pthread_mutex_init(&init->flag_died_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for death check\n");
		free(init);
		return (NULL);
	}
	return (init);
}

t_init	*init_flag_all_eat_mutex(t_init *init)
{
	if (pthread_mutex_init(&init->flag_all_eat_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for death check\n");
		free(init);
		return (NULL);
	}
	return (init);
}

t_init	*init_death_printed_mutex(t_init *init)
{
	if (pthread_mutex_init(&init->death_printed_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for death printed\n");
		free(init);
		return (NULL);
	}
	return (init);
}

t_init	*init_time_last_eat_mutex(t_init *init)
{
	int	i;

	i = 0;
	while (i < init->nb_of_philo)
	{
		if (pthread_mutex_init(&init->philo[i].time_last_eat_mutex, NULL) != 0)
		{
			printf("Failed to initialize mutex for time_last_eat_mutex\n");
			free(init);
			return (NULL);
		}
		i++;
	}
	return (init);
}
