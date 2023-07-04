/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/04 12:11:48 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	action_think(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
	}
	print_action(philo, init, philo->philo_id, "is thinking");
	return (1);
}

int action_eat(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
	}
	pthread_mutex_lock(&init->death_mutex);
	if (init->nb_must_eat != 0)
	{
		philo->nb_time_eat++;
		if (philo->nb_time_eat >= init->nb_must_eat)
		{
			pthread_mutex_lock(&init->eat_count_mutex);
			init->all_finished_eating++;
			pthread_mutex_unlock(&init->eat_count_mutex);
			if (init->all_finished_eating == init->nb_of_philo)
			{
				init->end_flag = 1;
				print_action(philo, init, philo->philo_id, "is eating");
				printf("%sAll Philosophers eat %d/%d. stopping program\n%s", RED, philo->nb_time_eat, init->nb_must_eat, RESET);
				pthread_mutex_unlock(&init->death_mutex);
				stop_all_if_flag(init);
				return (1);
			}
			print_action(philo, init, philo->philo_id, "is eating");
			pthread_mutex_unlock(&init->death_mutex);
			return (1);
		}
	}
	print_action(philo, init, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	pthread_mutex_unlock(&init->death_mutex);
	return (1);
}

int action_grab_fork(t_philo *philo, t_init *init)
{
	// print_action(philo, init, philo->philo_id, "has taken a fork"); // left fork 1
	// print_action(philo, init, philo->philo_id, "has taken a fork"); // right fork 2
	// usleep(init->time_to_eat * 1000);  // avancer pour eviter les deadlocks
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
	}
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		print_action(philo, init, philo->philo_id, "has taken a fork"); // left fork 1
		print_action(philo, init, philo->philo_id, "has taken a fork"); // right fork 2
		usleep(init->time_to_eat * 1000);  // avancer pour eviter les deadlocks
	}
	else 
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		print_action(philo, init, philo->philo_id, "has taken a fork"); // left fork 1
		print_action(philo, init, philo->philo_id, "has taken a fork"); // right fork 2
		usleep(init->time_to_eat * 1000);  // avancer pour eviter les deadlocks

	}
	// print_action(philo, init, philo->philo_id, "has taken a fork"); // left fork 1
	// print_action(philo, init, philo->philo_id, "has taken a fork"); // right fork 2

	// action_eat(philo, init);
	// usleep(init->time_to_eat * 1000);  // avancer pour eviter les deadlocks
	usleep(50);
	return (1);
}

int	action_drop_fork(t_philo *philo, t_init *init)
{
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
	}
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
	return (1);
}

int	action_sleep(t_philo *philo, t_init *init)
{
	print_action(philo, init, philo->philo_id, "is sleeping");
	usleep(init->time_to_sleep * 1000); // avancer pour eviter les deadlocks
	if (check_if_philo_died(philo, init) == 1)
	{
		init->end_flag = 1;
		stop_all_if_flag(init);
		return (0);
	}
	// print_action(philo, init, philo->philo_id, "is sleeping");
	// usleep(42);

	// usleep(init->time_to_sleep * 1000); // avancer pour eviter les deadlocks
	return (1);
}