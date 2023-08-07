/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/07 17:59:09 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	action_think(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if (init->nb_of_philo > 1)
	{
		print_action(init, philo->philo_id, "is thinking");
		ft_usleep(init->time_to_think, init);
	}
}

void	action_sleep(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if (init->nb_of_philo > 1)
	{
		print_action(init, philo->philo_id, "is sleeping");
		ft_usleep(init->time_to_sleep, init);
	}
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else if (philo->philo_id % 2 != 0)
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
}

void	action_eat(t_philo *philo, t_init *init)
{
	print_action(init, philo->philo_id, "is eating");
	pthread_mutex_lock(&(init->flag_all_eat_mutex));
	init->all_eat++;
	if (init->nb_must_eat != 0
		&& ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));
	philo->time_last_eat = get_time_philo();
	ft_usleep(init->time_to_eat, init);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if (init->nb_of_philo > 1)
	{
		if (philo->philo_id % 2 == 0)
		{
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			// check_time_for_philo_to_die(philo, init); // au lieu d avoir died a 400 pour ./philo 4 310 200 100 j'ai 500
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			// check_time_for_philo_to_die(philo, init);
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
		else if (philo->philo_id % 2 != 0)
		{
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			// check_time_for_philo_to_die(philo, init);
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			// check_time_for_philo_to_die(philo, init);
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
	}
}
