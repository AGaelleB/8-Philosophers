/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/26 18:34:36 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

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



t_init	*init_philo(t_init *data)
{
	int	i;

	i = data->nb_of_philo - 1;

	data->philo = malloc(sizeof(t_philo) * data->nb_of_philo);
	if (data->philo == NULL)
		return (NULL);
	while(i >= 0)
	{
		data->philo[i].philo_id = i;
		data->philo[i].nb_time_eat = 0;
		data->philo[i].left_fork_id = i;
		data->philo[i].right_fork_id = (i + 1) % data->nb_of_philo; // lili  Cela permet de connecter le dernier philosophe avec la première fourchette du tableau, formant ainsi une boucle.
		data->philo[i].time_last_eat = 0;

		// printf("%s***** INIT PHILO nb : '%d' *****\n", BLUE, i);
		// printf("philo_id = %d\n", data->philo[i].philo_id);
		// printf("nb_time_eat = %d\n", data->philo[i].nb_time_eat);
		// printf("left_fork_id = %d\n", data->philo[i].left_fork_id );
		// printf("right_fork_id = %d\n", data->philo[i].right_fork_id);
		// printf("time_last_eat = %lld\n", data->philo[i].time_last_eat);
		// printf("i = %d\n", i);
		// printf("********** END INIT ***********%s\n\n", RESET);
		
		i--;
	}
	return (data);
}

t_init	*init_recup_data(t_init *data, int ac, char **av)
{

	data = malloc(sizeof(t_init));
	if (data == NULL)
		return (NULL);
	data->nb_of_philo = ft_atoi_philo(av[1]);
	data->time_to_die = ft_atoi_philo(av[2]);
	data->time_to_eat = ft_atoi_philo(av[3]);
	data->time_to_sleep = ft_atoi_philo(av[4]);
	if(ac == 6)
		data->nb_must_eat = ft_atoi_philo(av[5]);
	else
		data->nb_must_eat = 0;
	// printf("%s***** INIT DATA *****\n", MAGENTA);
	// printf("nb_of_philo = %d\n", data->nb_of_philo);
	// printf("time_to_die = %d\n", data->time_to_die);
	// printf("time_to_eat = %d\n", data->time_to_eat);
	// printf("time_to_sleep = %d\n", data->time_to_sleep);
	// printf("nb_must_eat = %d\n", data->nb_must_eat);
	// printf("****** END INIT ******\n\n%s", RESET);

	return (data);
}
