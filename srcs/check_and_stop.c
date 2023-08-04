/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_and_stop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:15:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/04 17:10:27 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_flag_died(t_init *init)
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

int	check_flag_all_eat(t_init *init)
{
	pthread_mutex_lock(&init->flag_all_eat_mutex);
	if (init->flag_all_eat == 1)
	{
		pthread_mutex_unlock(&init->flag_all_eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(&init->flag_all_eat_mutex);
	return (0);
}

int	check_flag_death_printed(t_init *init)
{
	pthread_mutex_lock(&init->death_printed_mutex);
	if (init->death_printed == 1)
	{
		pthread_mutex_unlock(&init->death_printed_mutex);
		return (1);
	}
	pthread_mutex_unlock(&init->death_printed_mutex);
	return (0);
}

int	check_time_for_philo_to_die(t_philo *philo, t_init *init)
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

int	check_all_deaths(t_init *init)
{
	int	i;

	i = 0;
	while (i < init->nb_of_philo) // && (check_flag_died(init) == 0))
	{
		if (check_time_for_philo_to_die(&init->philo[i], init))
			return (1);
		i++;
	}
	return (0);
}
