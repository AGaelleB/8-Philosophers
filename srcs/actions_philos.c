/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/19 12:26:19 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
le code affiche plusiseurs fois les morts ou une action apres exemple ./philo 50 10 10 2   
aussi, lors de tous les repas pris il continue parfois la routine 
*/

#include "../includes/philo.h"

// int	action_take_fork(t_philo *philo, t_init *init)
// {
// 	if (check_and_stop_if_philo_died(philo, init) == 1)
// 		return (0);
// 	if (philo->philo_id % 2 == 0)
// 	{
// 		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
// 		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
// 		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
// 	}
// 	if (check_and_stop_if_philo_died(philo, init) == 1)
// 		return (0);
// 	print_action(philo, init, philo->philo_id, "has taken a fork");
// 	print_action(philo, init, philo->philo_id, "has taken a fork");
// 	action_eat(philo, init);
// 	return (1);
// }

// int	action_eat(t_philo *philo, t_init *init)
// {
// 	if (check_and_stop_if_philo_died(philo, init) == 1)
// 		return (0);
// 	print_action(philo, init, philo->philo_id, "is eating");
// 	if (init->nb_must_eat != 0)
// 	{
// 		philo->nb_time_eat++;
// 		if (philo->nb_time_eat >= init->nb_must_eat)
// 		{
// 			pthread_mutex_lock(&init->eat_count_mutex);
// 			init->all_finished_eating++;
// 			pthread_mutex_unlock(&init->eat_count_mutex);
// 			if (init->all_finished_eating == init->nb_of_philo)
// 			{
// 				pthread_mutex_lock(&init->death_mutex);
// 				print_action(philo, init, philo->philo_id, "TOUT LE MONDE A GRAILLE !!!!");
// 				init->end_flag = 1;
// 				cleanup_all_mutex(init);
// 				pthread_mutex_unlock(&init->death_mutex);
// 				exit(0);
// 			}
// 		}
// 	}
// 	if (check_and_stop_if_philo_died(philo, init) == 1)
// 		return (0);
// 	philo->time_last_eat = get_time_philo();
// 	usleep(init->time_to_eat * 1000);
// 	action_drop_fork(philo, init);
// 	return (1);
// }

// int	action_drop_fork(t_philo *philo, t_init *init)
// {
// 	if (philo->philo_id % 2 == 0)
// 	{
// 		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
// 		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
// 	}
// 	else
// 	{
// 		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
// 		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
// 	}
// 	return (1);
// }

// int	action_sleep(t_philo *philo, t_init *init)
// {
// 	if (check_and_stop_if_philo_died(philo, init) == 1)
// 		return (0);
// 	print_action(philo, init, philo->philo_id, "is sleeping");
// 	usleep(init->time_to_sleep * 1000);
// 	return (1);
// }

// int	action_think(t_philo *philo, t_init *init)
// {
// 	if (check_and_stop_if_philo_died(philo, init) == 1)
// 		return (0);
// 	print_action(philo, init, philo->philo_id, "is thinking");
// 	usleep(init->time_to_think * 1000);
// 		return (1);
// }

/*************************************************************************************/


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

void	action_think(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is thinking");
	usleep(init->time_to_think * 1000);
}


void	action_sleep(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is sleeping");
	usleep(init->time_to_sleep * 1000);
}

void	action_eat(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	philo->nb_time_eat++;
	usleep(init->time_to_eat * 1000);
	if (init->nb_must_eat != 0)
	{
		if (philo->nb_time_eat >= init->nb_must_eat)
		{
			pthread_mutex_lock(&init ->eat_count_mutex);
			init->all_finished_eating++;
			pthread_mutex_unlock(&init->eat_count_mutex);
			if (init->all_finished_eating == init->nb_of_philo)
			{
				pthread_mutex_lock(&init->death_mutex);
				print_action(philo, init, philo->philo_id, "END TOUT LE MONDE A GRAILLE !!!!");
				init->end_flag = 1;
				cleanup_all_mutex(init);
				pthread_mutex_unlock(&init->death_mutex);
				exit(0);
			}
		}
	}
	action_drop_fork(philo, init);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
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
	print_action(philo, init, philo->philo_id, "has taken a fork");
	print_action(philo, init, philo->philo_id, "has taken a fork");
	action_eat(philo, init);
}
