/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/21 17:57:07 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*thread_run(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		action_take_fork(data->philo, data->init);
		action_sleep(data->philo, data->init);
		action_think(data->philo, data->init);
	}
	return (NULL);
}

int	check_and_stop_if_philo_died(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		usleep(50); // permet de pas mourir fourchette a la main
		print_action(init, philo->philo_id, "died");
	}
	return (0);
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
	i = 0;
	// while (i < init->nb_of_philo)
	// {
	// 	free(init->philo[i].data);
	// 	i++;
	// }
	// free_all_data(init);
}

