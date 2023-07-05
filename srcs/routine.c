/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/05 14:29:29 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void stop_all_if_flag(t_init *init)
{
	pthread_mutex_lock(&init->death_mutex);
	if (init->end_flag == 1)
	{
		printf("%sPhilo died%s\n", RED, RESET);
		pthread_mutex_unlock(&init->death_mutex);
		exit(0);
	}
	pthread_mutex_unlock(&init->death_mutex);
}

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		pthread_mutex_lock(&init->death_mutex);
		print_action(philo, init, philo->philo_id, "died");
		init->end_flag = 1;
		exit (1); // magouille a free
		pthread_mutex_unlock(&init->death_mutex);
	}
	else
		return (0);
}

void *thread_routine(void *arg)
{
	t_data *data = (t_data *)arg;

	while (1)
	{
		if (!action_take_fork(data->philo, data->init))
			break;
		if (!action_sleep(data->philo, data->init))
			break;
		if (!action_think(data->philo, data->init))
			break;
	}
	return (NULL);
}

void	run_routine_philo(t_init *init)
{
	int	i;
	long long time_init;
	t_data *data;

	time_init = get_time_philo();

	i = init->nb_of_philo - 1;
	while(i >= 0)
	{
		stop_all_if_flag(init);
		data = malloc(sizeof(t_data));
		if (data == NULL)
			return ;
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_init = time_init;
		data->philo->time_last_eat = get_time_philo();
		pthread_create(&init->philo[i].thread_philo, NULL, thread_routine, data);
		i--;
	}
	// while (!init->end_flag)
	// 	usleep(100); // wait for some time, then check again
	i = init->nb_of_philo - 1;
	while (i >= 0) 
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i--;
	}
}
