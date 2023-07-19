/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:36:34 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/19 11:46:47 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_init	*init_recup_data(t_init *data, int ac, char **av)
{
	int time_to_think;


	data = malloc(sizeof(t_init));
	if (data == NULL)
		return (NULL);
	data->nb_of_philo = ft_atoi_philo(av[1]);
	data->time_to_die = ft_atoi_philo(av[2]);
	data->time_to_eat = ft_atoi_philo(av[3]);
	data->time_to_sleep = ft_atoi_philo(av[4]);
	time_to_think = (data->time_to_die - (data->time_to_eat + data->time_to_sleep)) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	data->time_to_think = time_to_think;

	data->all_finished_eating = 0;
	data->end_flag = 0;
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
	if (data->nb_of_philo == 1)
	{
		usleep(data->time_to_die * 1000);
		printf("%d %s died\n", ft_atoi_philo(av[2]) + 1, av[1]);
		free(data);
		exit (0);
	}
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
