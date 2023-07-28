/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/28 12:43:33 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	action_think(t_philo *philo, t_init *init)
{
	if (check_flag_died(init) || check_flag_all_eat(init))
		return;;
	print_action(init, philo->philo_id, "is thinking");
	usleep(init->time_to_think * 1000);
}

void	action_sleep(t_philo *philo, t_init *init)
{
	if (check_flag_died(init) || check_flag_all_eat(init))
		return;
	print_action(init, philo->philo_id, "is sleeping");
	usleep(init->time_to_sleep * 1000);
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
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
	if (check_flag_died(init) || check_flag_all_eat(init))
		return;

	print_action(init, philo->philo_id, "is eating");
	
	pthread_mutex_lock(&(philo->eat_mutex));
	init->all_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	
	if (init->nb_must_eat != 0 && ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		pthread_mutex_lock(&(init->flag_all_eat_mutex));
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	philo->time_last_eat = get_time_philo();
	usleep(init->time_to_eat * 1000); //en haut plus opti mais bug pour les repas
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	if (check_flag_died(init) || check_flag_all_eat(init))
		return;
		
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
	}
}
