/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/01 16:27:47 by abonnefo         ###   ########.fr       */
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
}

int	check_single_philo_and_exit(t_init *init, char **av)
{
	if (init->nb_of_philo == 1)
	{
		usleep(init->time_to_die * 1000);
		printf("%ld %s died\n", ft_atoi_philo(av[2]) + 1, av[1]);
		return (1);
	}
	return (0);
}

void	check_and_set_nb_must_eat(t_init *init, int ac, char **av)
{
	if (ac == 6)
		init->nb_must_eat = ft_atoi_philo(av[5]);
	else
		init->nb_must_eat = 0;
}

t_init	*init_recup_data(t_init *init, int ac, char **av)
{
	(void)ac;
	init = malloc(sizeof(t_init));
	if (init == NULL)
		return (NULL);
	init->nb_of_philo = ft_atoi_philo(av[1]);
	init->time_to_die = ft_atoi_philo(av[2]);
	init->time_to_eat = ft_atoi_philo(av[3]);
	init->time_to_sleep = ft_atoi_philo(av[4]);
	if (init->time_to_die >= INT_MAX || init->time_to_eat >= INT_MAX
		|| init->time_to_sleep >= INT_MAX)
	{
		free(init);
		return (NULL);
	}
	init->all_eat = 0;
	init->flag_all_eat = 0;
	init->flag_died = 0;
	init->death_printed = 0;
	calculate_and_set_time_to_think(init);
	check_and_set_nb_must_eat(init, ac, av);
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
		pthread_mutex_init(&(init->philo[i].eat_mutex), NULL);
		i--;
	}
	return (init);
}
