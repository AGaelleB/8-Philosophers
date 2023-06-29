/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/29 13:44:46 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void stop_all_if_flag(t_init *init)
{
	pthread_mutex_lock(&init->death_mutex);
	if (init->end_flag == 1)
	{
		pthread_mutex_unlock(&init->death_mutex);
		printf("%sEnd flag is set : %d stopping program%s\n", RED, init->end_flag == 1, RESET);
		exit(0);
	}
	pthread_mutex_unlock(&init->death_mutex);
}


int	check_if_philo_died(t_philo *philo, t_init *init)
{
	if (philo->time_last_eat >= init->time_to_die)
	{
		print_action(philo, philo->philo_id, "died");
		pthread_mutex_lock(&init->death_mutex);
		init->end_flag = 1;
		pthread_mutex_unlock(&init->death_mutex);
		return (init->end_flag = 1);
	}
	else
		return (init->end_flag = 0);
}


void *thread_routine(void *arg)
{
	t_data *data = (t_data *)arg;

	while (check_if_philo_died(data->philo, data->init) == 0)
	{
		stop_all_if_flag(data->init);
		action_grab_fork(data->philo, data->init);
		action_drop_fork(data->philo, data->init);
		action_sleep(data->philo, data->init);
		action_think(data->philo, data->init);
	}
	return (NULL);
}

void	run_routine_philo(t_init *init)
{
	int	i;
	long long time_init;
	t_data *data;

	time_init = get_time_philo();

	// création des threads par philosophers
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
		// data->philo[i].init_data = init; // utile ? 
		pthread_create(&init->philo[i].thread_philo, NULL, thread_routine, data);
		i--;
	}
	
	i = init->nb_of_philo - 1;
	// pthread_join attend que tous les threads se terminent avant de continuer
	while (i >= 0) 
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i--;
	}
}
