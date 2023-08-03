/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/03 10:32:46 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void action_think(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
	// if (init->death_printed == 0)
	// {
	// 	printf("%s action_think = %d%s\n",YELLOW, init->death_printed, RESET );
		print_action(init, philo->philo_id, "is thinking");
	// }
	usleep(init->time_to_think * 1000);


	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
}

void action_sleep(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
	// if (init->death_printed == 0)
	// {
	// 	printf("%s action_sleep = %d%s\n",YELLOW, init->death_printed, RESET );
		print_action(init, philo->philo_id, "is sleeping");
	// }
	usleep(init->time_to_sleep * 1000);


	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
}

void action_drop_fork(t_philo *philo, t_init *init)
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

void action_eat(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
	// if (init->death_printed == 0)
	// {
	// 	printf("%s action_eat = %d%s\n",YELLOW, init->death_printed, RESET );
		print_action(init, philo->philo_id, "is eating");
	// }
	pthread_mutex_lock(&(philo->eat_mutex));
	init->all_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	if (init->nb_must_eat != 0 && ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		pthread_mutex_lock(&(init->flag_all_eat_mutex));
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return;
	}
	philo->time_last_eat = get_time_philo();
	usleep(init->time_to_eat * 1000);


	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
}

void action_take_fork(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		action_eat(philo, init);
		action_drop_fork(philo, init);
	}
	else if (philo->philo_id % 2 != 0)
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		action_eat(philo, init);
		action_drop_fork(philo, init);
	}


	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return ;
}

// void action_take_fork(t_philo *philo, t_init *init)
// {
//     check_time_for_philo_to_die(philo, init);
//     if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
//         return;
//     if (philo->philo_id % 2 == 0)
//     {
//         pthread_mutex_lock(&init->forks[philo->left_fork_id]);
//         check_time_for_philo_to_die(philo, init);
//         // if (init->death_printed == 0)
//         // {
//         //     printf("%s1 action_take_fork = %d%s\n",YELLOW, init->death_printed, RESET );
//             print_action(init, philo->philo_id, "has taken a fork");
//         // }
//         if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init)) {
//             action_drop_fork(philo, init);
//             return;
//         }
//         pthread_mutex_lock(&init->forks[philo->right_fork_id]);
//         check_time_for_philo_to_die(philo, init);
//         // if (init->death_printed == 0)
//         // {
//         //     printf("%s1 action_take_fork = %d%s\n",YELLOW, init->death_printed, RESET );
//             print_action(init, philo->philo_id, "has taken a fork");
//         // }
//         if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init)) {
//             action_drop_fork(philo, init);
//             return;
//         }
//         action_eat(philo, init);
//         action_drop_fork(philo, init);
//     }    
// 	else if (philo->philo_id % 2 != 0)
//     {
// 		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
//         check_time_for_philo_to_die(philo, init);
//         // if (init->death_printed == 0)
//         // {
//         //     printf("%s1 action_take_fork = %d%s\n",YELLOW, init->death_printed, RESET );
//             print_action(init, philo->philo_id, "has taken a fork");
//         // }
//         if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init)) 
// 		{
//             action_drop_fork(philo, init);
//             return;
//         }
//         pthread_mutex_lock(&init->forks[philo->left_fork_id]);
//         check_time_for_philo_to_die(philo, init);
//         // if (init->death_printed == 0)
//         // {
//         //     printf("%s1 action_take_fork = %d%s\n",YELLOW, init->death_printed, RESET );
//             print_action(init, philo->philo_id, "has taken a fork");
//         // }
//         if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init)) 
// 		{
//             action_drop_fork(philo, init);
//             return;
//         }
//         action_eat(philo, init);
//         action_drop_fork(philo, init);
//     }
// }