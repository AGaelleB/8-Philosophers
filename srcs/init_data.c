/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/19 19:18:09 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	calculate_and_set_time_to_think(t_init *data)
{
	int	time_to_eat_and_sleep;
	int	time_to_think;

	time_to_eat_and_sleep = data->time_to_eat + data->time_to_sleep;
	time_to_think = (data->time_to_die - (time_to_eat_and_sleep)) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	data->time_to_think = time_to_think;
	data->all_finished_eating = 0;
}

void	check_single_philo_and_exit(t_init *data, char **av)
{
	if (data->nb_of_philo == 1)
	{
		usleep(data->time_to_die * 1000);
		printf("%d %s died\n", ft_atoi_philo(av[2]) + 1, av[1]);
		free(data);
		exit (0);
	}
}

void	check_and_set_nb_must_eat(t_init *data, int ac, char **av)
{
	if (ac == 6)
	{
		data->nb_must_eat = ft_atoi_philo(av[5]);
		if (data->nb_must_eat == 0)
		{
			write(2, "", 1);
			free(data);
			exit (0);
		}
	}
	else
		data->nb_must_eat = 0;
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
	return (data);
	calculate_and_set_time_to_think(data);
	check_and_set_nb_must_eat(data, ac, av);
	check_single_philo_and_exit(data, av);
	return (data);
}

t_init	*init_philo(t_init *data)
{
	int	i;

	i = data->nb_of_philo - 1;
	data->philo = malloc(sizeof(t_philo) * (data->nb_of_philo));
	if (data->philo == NULL)
		return (NULL);
	while (i >= 0)
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
