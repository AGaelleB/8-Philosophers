/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/29 13:46:24 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	action_think(t_philo *philo, t_init *init)
{

	stop_all_if_flag(init);
	
	// printf("%sin action_think init->end_flag = %d\n%s", GREEN, init->end_flag, RESET);  // DEBUG

	// usleep(init->time_to_sleep * 1000);

	// printf("%sphilo->time_init = %lld\n%s", YELLOW, philo->, RESET);
	print_action(philo, philo->philo_id, "is thinking");
}

void action_eat(t_philo *philo, t_init *init)
{

	stop_all_if_flag(init);


	pthread_mutex_lock(&init->death_mutex);
	if (init->nb_must_eat != 0)
	{
		philo->nb_time_eat++;
		if (philo->nb_time_eat >= init->nb_must_eat)
		{
			print_action(philo, philo->philo_id, "is eating");
			init->end_flag = 1;
			printf("%sPhilosopher n.%d eat %d/%d. stopping program\n%s", RED, philo->philo_id, philo->nb_time_eat, init->nb_must_eat, RESET);
			// printf("%sin action_eat data->init->end_flag = %d\n%s", GREEN, init->end_flag, RESET);  // DEBUG
			// mettre fin au programme des que le 1er repas aura ete pris
			pthread_mutex_unlock(&init->death_mutex);
			return ;
		}
	}
	print_action(philo, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	pthread_mutex_unlock(&init->death_mutex);
}

void	action_grab_fork(t_philo *philo, t_init *init)
{
	stop_all_if_flag(init);

	// printf("%sin action_grab_fork init->end_flag = %d\n%s", GREEN, init->end_flag, RESET);  // DEBUG
	
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
	}
	else 
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
	}
	print_action(philo, philo->philo_id, "has taken a fork"); // left fork 1
	print_action(philo, philo->philo_id, "has taken a fork"); // right fork 2
	action_eat(philo, init);
	action_drop_fork(philo, init);
}

void	action_drop_fork(t_philo *philo, t_init *init)
{

	stop_all_if_flag(init);

	// printf("%sin action_drop_fork init->end_flag = %d\n%s", GREEN, init->end_flag, RESET);  // DEBUG

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
}

void	action_sleep(t_philo *philo, t_init *init)
{

	stop_all_if_flag(init);

	// printf("%sin action_sleep init->end_flag = %d\n%s", GREEN, init->end_flag, RESET);  // DEBUG


	// printf("DEBUT time sleep de philo n.%d = %d\n", philo->philo_id,  init->time_to_sleep);
	usleep(init->time_to_sleep * 1000);
	// printf("FIN time sleep de philo n.%d = %d\n\n", philo->philo_id, init->time_to_sleep);

	print_action(philo, philo->philo_id, "is sleeping");
}
