/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/27 20:00:24 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int check_flag_died(t_init *init)
{
	pthread_mutex_lock(&(init->flag_died_mutex));
	if (init->flag_died == 1)
	{
		pthread_mutex_unlock(&init->flag_died_mutex);
		return (1);
	}
	pthread_mutex_unlock(&init->flag_died_mutex);
	return (0);
}

int check_flag_all_eat(t_init *init)
{
	pthread_mutex_lock(&init->flag_all_eat_mutex);
	if (init->flag_all_eat == 1)
	{
		pthread_mutex_unlock(&init->flag_all_eat_mutex);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&init->flag_all_eat_mutex);
	return (0);
}

int check_time_for_philo_to_die(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		pthread_mutex_lock(&(init->flag_died_mutex));
		init->flag_died = 1;
		pthread_mutex_unlock(&(init->flag_died_mutex));
		print_action(init, philo->philo_id, "died");
		return (1);
	}
	return (0);
}

void *thread_run(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (data->init->flag_died != 1 && data->init->flag_all_eat != 1)
	{
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_take_fork(data->philo, data->init);
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_eat(data->philo, data->init);
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_drop_fork(data->philo, data->init);
		if (check_time_for_philo_to_die(data->philo, data->init))
			break;
		action_sleep(data->philo, data->init);
		if (check_time_for_philo_to_die(data->philo, data->init))
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
		// usleep(150); // permet d'avoir de ne pas bug dans la prise de forks
		i++;
	}
}

void run_routine_philo(t_init *init)
{
	long long int	time_init;
	int				i;

	time_init = get_time_philo();
	i = 0;
	init_run_philosophers(init, time_init);
	while ((i < init->nb_of_philo) && (init->flag_died != 1) && (init->flag_all_eat != 1))
	{
		// if (check_flag_all_eat(init))
		// {
		// 	// faire des free
		// 	return ;
		// }
		if (check_flag_died(init))
		{
			check_time_for_philo_to_die(init->philo, init); // permet d'arrêter de créer de nouveaux threads si un philo meurt avant que tous les threads soient créés
			// pthread_mutex_destroy(&init->flag_died_mutex);
			// pas free des threads crees si fin prematuree
			return ;
		}
		pthread_join(init->philo[i].thread_philo, NULL);
		free(init->philo[i].data); // m'a supprime des leaks !
		i++;
	}
}

