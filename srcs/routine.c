/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/19 19:10:25 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	routine_one(t_init*init, t_philo *philo)
{
	while (philo->nb_time_eat != init->nb_must_eat)
	{
		action_take_fork(philo, init);
		if (philo->nb_time_eat == init->nb_must_eat)
			break ;
		check_all_deaths(init);
		action_sleep(philo, init);
		check_all_deaths(init);
		action_think(philo, init);
		check_all_deaths(init);
	}
}

void	routine_two(t_init *init, t_philo *philo)
{
	while (1)
	{
		action_take_fork(philo, init);
		check_all_deaths(init);
		action_sleep(philo, init);
		check_all_deaths(init);
		action_think(philo, init);
		check_all_deaths(init);
	}
}

void	*thread_run(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (data->init->nb_must_eat > 0)
		routine_one(data->init, data->philo);
	else
		routine_two(data->init, data->philo);
	return (NULL);
}

void	init_run_philosophers(t_init *init, long long int time_init)
{
	t_data	*data;
	int		i;

	i = 0;
	while (i < init->nb_of_philo)
	{
		data = malloc(sizeof(t_data));
		if (data == NULL)
			return ;
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_init = time_init;
		data->philo->time_last_eat = get_time_philo();
		init->philo[i].data = data;
		pthread_create(&init->philo[i].thread_philo, NULL, thread_run, data);
		i++;
	}
}

void	run_routine_philo(t_init *init)
{
	long long int	time_init;
	int				i;

	time_init = get_time_philo();
	i = 0;
	init_run_philosophers(init, time_init);
	while (i < init->nb_of_philo)
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i++;
	}
	// ajouter une fonction pour free
	// free_all_data(init);
}
