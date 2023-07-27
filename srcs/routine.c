/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/27 10:31:13 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_end_flag(t_init *init)
{
	pthread_mutex_lock(&init->end_flag_mutex);
	if (init->end_flag == 1)
	{
		pthread_mutex_unlock(&init->end_flag_mutex);
		return (1);
	}
	pthread_mutex_unlock(&init->end_flag_mutex);
	return (0);
}

int check_and_stop_if_philo_died(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		print_action(init, philo->philo_id, "died");
		pthread_mutex_lock(&(init->end_flag_mutex));
		init->end_flag = 1;
		pthread_mutex_unlock(&(init->end_flag_mutex));
		return (1);
	}
	return (0);
}

void *thread_run(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (data->init->end_flag != 1)
	{
		if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
			break;
		action_take_fork(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
			break;
		action_eat(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
			break;
		action_drop_fork(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
			break;
		action_sleep(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
			break;
		action_think(data->philo, data->init);
	}
	return (NULL);
}

void init_run_philosophers(t_init *init, long long int time_init)
{
	t_data *data;
	int i;

	i = 0;
	// if (init->end_flag != 1) // pas utile je crois
	// {
		while ((i < init->nb_of_philo))
		{
			data = malloc(sizeof(t_data));
			if (data == NULL)
				return;
			data->init = init;
			data->philo = &init->philo[i];
			data->philo->time_last_eat = get_time_philo();
			init->philo[i].data = data;
			data->philo->time_init = time_init;
			if (pthread_create(&init->philo[i].thread_philo, NULL, thread_run, data))
				return ;
			usleep(100); // permet d'avoir moins de message apres le end ?
			i++;
		}
	// }
}

void run_routine_philo(t_init *init)
{
	long long int	time_init;
	int				i;

	time_init = get_time_philo();
	i = 0;
	init_run_philosophers(init, time_init);
	while (i < init->nb_of_philo)
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		free(init->philo[i].data); // m'a supprime mes derniers leaks !!!
		i++;
	}
	// usleep(100); // permet de pas avoir de doublons de died ?
}
