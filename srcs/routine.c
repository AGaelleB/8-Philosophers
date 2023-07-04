/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/04 10:56:03 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void stop_all_if_flag(t_init *init)
{
	pthread_mutex_lock(&init->death_mutex);
	if (init->end_flag == 1)
	{
		printf("%sEnd flag is set : %d. stopping program%s\n", RED, init->end_flag == 1, RESET);
		pthread_mutex_unlock(&init->death_mutex);
		exit(0);
	}
	pthread_mutex_unlock(&init->death_mutex);
}

int	check_if_philo_died(t_philo *philo, t_init *init)
{
	usleep(50); // test
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		print_action(philo, init, philo->philo_id, "died");
		pthread_mutex_lock(&init->death_mutex);
		init->end_flag = 1;
		pthread_mutex_unlock(&init->death_mutex);
		return (1);
	}
	else
		return (0);
}

void *thread_routine(void *arg)
{
	t_data *data = (t_data *)arg;

	while (1)
	{
		action_grab_fork(data->philo, data->init);
		action_eat(data->philo, data->init);
		action_drop_fork(data->philo, data->init);
		action_sleep(data->philo, data->init);
		action_think(data->philo, data->init);
		
		// if (action_grab_fork(data->philo, data->init) == 0) 
		// {
		// 	action_eat(data->philo, data->init); // libérer les ressources avant de se terminer
		// 	return NULL;
		// }
		// if (action_grab_fork(data->philo, data->init) == 0) 
		// {
		// 	action_drop_fork(data->philo, data->init); // libérer les ressources avant de se terminer
		// 	return NULL;
		// }
		// if (action_sleep(data->philo, data->init) == 0) 
		// {
		// 	action_sleep(data->philo, data->init); // libérer les ressources avant de se terminer
		// 	return NULL;
		// }
		// if (action_think(data->philo, data->init) == 0) 
		// {
		// 	action_think(data->philo, data->init); // libérer les ressources avant de se terminer
		// 	return NULL;
		// }
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
		data->philo->time_last_eat = get_time_philo();
		pthread_create(&init->philo[i].thread_philo, NULL, thread_routine, data);
		i--;
	}
	// ADD LILI
	while (!init->end_flag)
		usleep(100); // wait for some time, then check again
	
	i = init->nb_of_philo - 1;
	// pthread_join attend que tous les threads se terminent avant de continuer
	while (i >= 0) 
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i--;
	}
}