/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/04 17:30:39 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**********************************************************/

// void ft_usleep(int time_usleep, t_philo *philo, t_init *init)
// {
// 	long long start = get_time_philo();

// 	while(get_time_philo() < (start + time_usleep) >  && !check_time_for_philo_to_die(philo, init))
// 	{
// 		usleep(50);
// 	}
// }

// void ft_usleep(int time_usleep, t_philo *philo, t_init *init)
// {
// 	long long start = get_time_philo();

// 	while((start + time_usleep) > get_time_philo() && !((get_time_philo() - philo->time_last_eat) > init->time_to_die))
// 	{
// 		usleep(50);
// 	}
// }


// void ft_usleep(int time_usleep)
// {
// 	long long start = get_time_philo();

// 	while(get_time_philo() < (start + time_usleep))
// 	{
// 		usleep(100);
// 	}
// }



/**********************************************************/


void	action_think(t_philo *philo, t_init *init)
{

	// check_all_deaths(init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;

	if (init->nb_of_philo > 1)
	{
		check_time_for_philo_to_die(philo, init);

		long long int time = get_time_philo(); // ajout

		if ((time + init->time_to_die) > (init->time_to_think * 1000)) // ajout
		{
			usleep((init->time_to_die - init->time_to_eat) * 1000); // ajout
			check_time_for_philo_to_die(philo, init); // ajout
		}
		check_time_for_philo_to_die(philo, init);
		if (check_flag_died(init) || check_flag_all_eat(init))
			return ;
		print_action(init, philo->philo_id, "is thinking");
		usleep(init->time_to_think);
	}
}

void	action_sleep(t_philo *philo, t_init *init)
{

	// check_all_deaths(init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;

	if (init->nb_of_philo > 1)
	{
		check_time_for_philo_to_die(philo, init);

		long long int time = get_time_philo(); // ajout

		if ((time + init->time_to_die) > (init->time_to_sleep * 1000)) // ajout
		{
			usleep((init->time_to_die - init->time_to_eat) * 1000); // ajout
			check_time_for_philo_to_die(philo, init); // ajout
		}
		
		check_time_for_philo_to_die(philo, init);
		if (check_flag_died(init) || check_flag_all_eat(init))
			return ;
		print_action(init, philo->philo_id, "is sleeping");
		usleep(init->time_to_sleep * 1000);
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
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));
	pthread_mutex_lock(&(init->flag_all_eat_mutex));
	if (init->nb_must_eat != 0
		&& ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));

	philo->time_last_eat = get_time_philo();
	usleep(init->time_to_eat * 1000);

}

void	action_take_fork(t_philo *philo, t_init *init)
{
	long long int time = get_time_philo(); // ajout
	if ((time + init->time_to_die) > (init->time_to_eat * 1000)) // ajout
	{
		// usleep((init->time_to_die - init->time_to_eat) * 1000); // ajout
		check_time_for_philo_to_die(philo, init); // ajout
	}

	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if (init->nb_of_philo > 1)
	{
		if (philo->philo_id % 2 == 0)
		{
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			check_time_for_philo_to_die(philo, init);
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
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
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
	}
}
