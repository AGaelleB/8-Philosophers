/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/19 12:25:28 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// int	check_and_stop_if_philo_died(t_philo *philo, t_init *init)
// {
// 	int ret = 0;

// 	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
// 	{
// 		pthread_mutex_lock(&init->death_mutex);
// 		init->end_flag = 1;
// 		ret = 1;
// 		pthread_mutex_unlock(&init->death_mutex);
// 	}
// 	if (ret == 1)
// 	{
// 		pthread_mutex_lock(&init->death_mutex);
// 		print_action(philo, init, philo->philo_id, "died");
// 		print_action(philo, init, philo->philo_id, "IL EST MORT OK C'EST FINI MAINTENANT !!!!");
// 		cleanup_all_mutex(init);
// 		pthread_mutex_unlock(&init->death_mutex);
// 		exit(0);
// 	}
// 	return ret;
// }

// void *thread_run(void *arg)
// {
// 	t_data *data;

// 	data = (t_data *)arg;
// 	if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
// 			return NULL;
// 	while (!data->init->end_flag) // ou boucle (1)
// 	{
// 		if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
// 			return NULL;
// 		action_take_fork(data->philo, data->init);
// 		if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
// 			return NULL;
// 		action_sleep(data->philo, data->init);
// 		if (check_and_stop_if_philo_died(data->philo, data->init) == 1)
// 			return NULL;
// 		action_think(data->philo, data->init);
// 	}
// 	return (NULL);
// }

// void	run_routine_philo(t_init *init)
// {
// 	int			i;
// 	long long	time_init;
// 	t_data		*data;

// 	time_init = get_time_philo();
// 	i = init->nb_of_philo - 1;
// 	while (i >= 0)
// 	{
// 		data = malloc(sizeof(t_data));
// 		if (data == NULL)
// 			return ;
// 		data->init = init;
// 		data->philo = &init->philo[i];
// 		data->philo->time_init = time_init;
// 		data->philo->time_last_eat = get_time_philo();
// 		if(pthread_create(&init->philo[i].thread_philo, NULL, thread_run, data) != 0) 
// 		{
// 			free(data);
// 			return ;
// 		}
// 		i--;
// 	}
// 	i = init->nb_of_philo - 1;
// 	while (i >= 0)
// 	{
// 		pthread_join(init->philo[i].thread_philo, NULL);
// 		i--;
// 	}
// 	cleanup_all(init); // Utiliser la fonction cleanup_all pour libérer la mémoire
// }

void	check_all_deaths(t_philo *philo, t_init *init)
{
	int ret = 0;

	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		pthread_mutex_lock(&init->death_mutex);
		init->end_flag = 1;
		ret = 1;
		pthread_mutex_unlock(&init->death_mutex);
	}
	if (ret == 1)
	{
		pthread_mutex_lock(&init->death_mutex);
		print_action(philo, init, philo->philo_id, "died");
		print_action(philo, init, philo->philo_id, "IL EST MORT OK C'EST FINI MAINTENANT !!!!");
		cleanup_all_mutex(init);
		pthread_mutex_unlock(&init->death_mutex);
		exit(0);
	}
}

void	routine_one(t_init*init, t_philo *philo)
{
	int	i;

	i = 1;
	while (i <= init->nb_must_eat)
	{
		action_take_fork(philo, init);
		if(i == init->nb_must_eat)
			break;
		check_all_deaths(philo, init);
		action_sleep(philo, init);
		check_all_deaths(philo, init);
		action_think(philo, init);
		check_all_deaths(philo, init);
		i++;
	}
}

void routine_two(t_init *init, t_philo *philo)
{

	while (1)
	{
		action_take_fork(philo, init);
		check_all_deaths(philo, init);
		action_sleep(philo, init);
		check_all_deaths(philo, init);
		action_think(philo, init);
		check_all_deaths(philo, init);
	}
}

void	*thread_run(void *arg)
{
	t_data *data = (t_data *)arg;

	if(data->init->nb_must_eat > 0)
		routine_one(data->init, data->philo);
	else 
		routine_two(data->init, data->philo);
	return(NULL);
	// pthread_exit(NULL);
}


void run_routine_philo(t_init *init) 
{
	int i = 0;
	long long int time_init = get_time_philo();
	t_data *data = NULL;

	while (i < init->nb_of_philo)
	{
		data = malloc(sizeof(t_data));
		if (!data)
		{
			int j = 0;
			while (j < i)
			{
				free(init->philo[j].data);
				j++;
			}
			return;
		}
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_init = time_init;
		data->philo->time_last_eat = get_time_philo();
		init->philo[i].data = data;
		pthread_create(&init->philo[i].thread_philo, NULL, thread_run, data);
		i++;
	}
	i = 0;
	while (i < init->nb_of_philo)
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i++;
	}
	i = 0;
	while (i < init->nb_of_philo)
	{
		free(init->philo[i].data);
		i++;
	}
}
