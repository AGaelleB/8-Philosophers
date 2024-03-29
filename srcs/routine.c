/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/08 10:23:13 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*thread_run(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_flag_died(data->init) || check_flag_all_eat(data->init)
			|| check_flag_death_printed(data->init))
			return (NULL);
		action_take_fork(data->philo, data->init);
		if (check_flag_died(data->init) || check_flag_all_eat(data->init)
			|| check_flag_death_printed(data->init))
			return (NULL);
		action_sleep(data->philo, data->init);
		if (check_flag_died(data->init) || check_flag_all_eat(data->init)
			|| check_flag_death_printed(data->init))
			return (NULL);
		action_think(data->philo, data->init);
	}
	return (NULL);
}

void	init_and_create_threads(t_init *init, long long int time_init)
{
	t_data	*data;
	int		i;

	i = 0;
	while ((i < init->nb_of_philo))
	{
		data = malloc(sizeof(t_data));
		if (data == NULL)
			return ;
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_last_eat = get_time_philo();
		init->philo[i].data = data;
		data->philo->time_init = time_init;
		if (pthread_create(&init->philo[i].thread_philo, NULL,
				thread_run, data))
			return ;
		usleep(50);
		i++;
	}
}

void	run_routine_philo(t_init *init)
{
	long long int	time_init;
	int				i;

	time_init = get_time_philo();
	i = 0;
	init_and_create_threads(init, time_init);
	while ((i < init->nb_of_philo))
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		free(init->philo[i].data);
		i++;
	}
}
