/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/21 18:41:08 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	action_think(t_philo *philo, t_init *init)
{
	print_action(init, philo->philo_id, "is thinking");
	check_and_stop_if_philo_died(philo, init);
	usleep(init->time_to_think * 1000);
	check_and_stop_if_philo_died(philo, init);
}

void	action_sleep(t_philo *philo, t_init *init)
{
	print_action(init, philo->philo_id, "is sleeping");
	check_and_stop_if_philo_died(philo, init);
	usleep(init->time_to_sleep * 1000);
	check_and_stop_if_philo_died(philo, init);
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	check_and_stop_if_philo_died(philo, init);
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
}

void	action_eat(t_philo *philo, t_init *init)
{
	int	i;

	i = 0;
	print_action(init, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	pthread_mutex_lock(&(philo->eat_mutex));
	init->philo[i].nb_time_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	if (init->nb_must_eat != 0)
	{
		if ((init->nb_of_philo * init->nb_must_eat)
			== init->philo[i].nb_time_eat)
		{
			init->end_flag = 1;
			check_and_stop_if_philo_died(philo, init);
			exit (-1);
		}
		init->all_eat++;
	}
	i++;
	usleep(init->time_to_eat * 1000);
	action_drop_fork(philo, init);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	check_and_stop_if_philo_died(philo, init);
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
	}
	else
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
	}
	print_action(init, philo->philo_id, "has taken a fork");
	print_action(init, philo->philo_id, "has taken a fork");
	check_and_stop_if_philo_died(philo, init);
	action_eat(philo, init);
}
