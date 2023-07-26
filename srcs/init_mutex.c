/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/26 16:06:11 by abonnefo         ###   ########.fr       */
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

t_init	*init_end_flag_mutex(t_init *init)
{
	if (pthread_mutex_init(&init->end_flag_mutex, NULL) != 0)
	{
		printf("Failed to initialize mutex for death check\n");
		free(init);
		return (NULL);
	}
	return (init);
}

t_init	*init_forks(t_init *init)
{
	int	i;

	init->forks = malloc(sizeof(pthread_mutex_t) * init->nb_of_philo);
	if (init->forks == NULL)
		return (NULL);
	i = init->nb_of_philo - 1;
	while (i >= 0)
	{
		if (pthread_mutex_init(&init->forks[i], NULL) != 0)
		{
			while (i < init->nb_of_philo)
			{
				pthread_mutex_destroy(&init->forks[i]);
				i++;
			}
			free(init->forks);
			return (NULL);
		}
		i--;
	}
	return (init);
}
