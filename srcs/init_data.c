/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/20 17:23:22 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	calculate_and_set_time_to_think(t_init *init)
{
	int	time_to_eat_and_sleep;
	int	time_to_think;

	time_to_eat_and_sleep = init->time_to_eat + init->time_to_sleep;

	time_to_think = (init->time_to_die - (time_to_eat_and_sleep)) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	init->time_to_think = time_to_think;
	init->all_finished_eating = 0;
}

void	check_single_philo_and_exit(t_init *init, char **av)
{
	if (init->nb_of_philo == 1)
	{
		usleep(init->time_to_die * 1000);
		printf("%d %s died\n", ft_atoi_philo(av[2]) + 1, av[1]);
		// free(init);
		exit (0);
	}
}

void	check_and_set_nb_must_eat(t_init *init, int ac, char **av)
{
	if (ac == 6)
	{
		init->nb_must_eat = ft_atoi_philo(av[5]);
		if (init->nb_must_eat == 0)
		{
			write(2, "", 1);
			// free(init);
			exit (0);
		}
	}
	else
		init->nb_must_eat = 0;
}

t_init *init_recup_data(t_init *init, int ac, char **av)
{
	init = malloc(sizeof(t_init));
	if (init == NULL)
		return (NULL);
	init->nb_of_philo = ft_atoi_philo(av[1]);
	init->time_to_die = ft_atoi_philo(av[2]);
	init->time_to_eat = ft_atoi_philo(av[3]);
	init->time_to_sleep = ft_atoi_philo(av[4]);
	calculate_and_set_time_to_think(init);
	check_and_set_nb_must_eat(init, ac, av);
	check_single_philo_and_exit(init, av);
	return (init);
}

t_init	*init_philo(t_init *init)
{
	int	i;

	i = init->nb_of_philo - 1;
	init->philo = malloc(sizeof(t_philo) * (init->nb_of_philo));
	if (init->philo == NULL)
		return (NULL);
	while (i >= 0)
	{
		init->philo[i].philo_id = i + 1;
		init->philo[i].nb_time_eat = 0;
		init->philo[i].left_fork_id = i;
		if (init->nb_of_philo == 1)
			init->philo[i].right_fork_id = (i + 1);
		else
			init->philo[i].right_fork_id = (i + 1) % init->nb_of_philo;
		init->philo[i].time_last_eat = 0;
		pthread_mutex_init(&(init->philo[i].eat_mutex), NULL); // ajout rapide ici
		i--;
	}
	return (init);
}


