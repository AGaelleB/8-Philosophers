/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/05 12:09:42 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_init	*init_recup_data(t_init *data, int ac, char **av)
{
	data = malloc(sizeof(t_init));
	if (data == NULL)
		return (NULL);
	data->nb_of_philo = ft_atoi_philo(av[1]);
	data->time_to_die = ft_atoi_philo(av[2]);
	data->time_to_eat = ft_atoi_philo(av[3]);
	data->time_to_sleep = ft_atoi_philo(av[4]);
	data->time_to_think = (data->time_to_die - (data->time_to_eat + data->time_to_sleep)) / 2;
	data->all_finished_eating = 0;
	if(ac == 6)
		data->nb_must_eat = ft_atoi_philo(av[5]);
	else
		data->nb_must_eat = 0;
	return (data);
}

t_init	*init_philo(t_init *data)
{
	int	i;

	i = data->nb_of_philo - 1;
	data->philo = malloc(sizeof(t_philo) * (data->nb_of_philo));
	if (data->philo == NULL)
	{
		free(data);
		return (NULL);
	}
	while(i >= 0)
	{
		data->philo[i].philo_id = i + 1;
		data->philo[i].nb_time_eat = 0;
		data->philo[i].left_fork_id = i;
		if (data->nb_of_philo == 1)
			data->philo[i].right_fork_id = (i + 1);
		else
			data->philo[i].right_fork_id = (i + 1) % data->nb_of_philo;
		data->philo[i].time_last_eat = 0;
		i--;
	}
	return (data);
}

t_init	*init_mutex(t_init *data)
{
	int	i;
	int	mutex;

	i = data->nb_of_philo - 1;
	while(i >= 0)
	{
		mutex = pthread_mutex_init(&data->philo[i].mutex, NULL);
		if (mutex != 0)
		{
			printf("Failed to initialize mutex for philosopher %d\n", i);
			free(data->philo);
			free(data);
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
		return (NULL);
	}
	return (data);
}

t_init	*init_forks(t_init *data)
{
	int i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_of_philo);
	if (data->forks == NULL)
		return (NULL);

	i = data->nb_of_philo - 1;
	while (i >= 0)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			// Ajout d'un nettoyage pour éviter les leaks
			while(i < data->nb_of_philo)
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
