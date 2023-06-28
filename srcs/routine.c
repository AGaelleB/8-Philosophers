/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/28 17:17:30 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	if (philo->time_last_eat >= init->time_to_die)
	{
		print_action(philo, philo->philo_id, "died");
		pthread_mutex_lock(&init->death_mutex);
		init->death_flag = 1;
		pthread_mutex_unlock(&init->death_mutex);
		return (philo->flag_end_routine = 1);
	}
	else
		return (philo->flag_end_routine = 0);
}

void *thread_routine(void *arg)
{
	t_data *data = (t_data *)arg;
	while (check_if_philo_died(data->philo, data->init) == 0)
	{
		if (data->philo->flag_end_routine == 1)
			break;
		action_grab_fork(data->philo, data->init);
		if (data->philo->flag_end_routine == 1)
			break;
		action_drop_fork(data->philo, data->init);
		if (data->philo->flag_end_routine == 1)
			break;
		action_sleep(data->philo, data->init);
		if (data->philo->flag_end_routine == 1)
			break;
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

	// creation des threads par philosophers
	i = init->nb_of_philo - 1;
	while(i >= 0)
	{
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




