/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/19 19:19:38 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	check_death(t_init *init, t_philo *philo)
{
	long long int	current_time;
	long long int	time_since_last_eat;
	int				end;

	current_time = get_time_philo();
	time_since_last_eat = current_time - philo->time_last_eat;
	end = 0;
	if (time_since_last_eat > (long long int)init->time_to_die)
	{
		pthread_mutex_lock(&init->death_mutex);
		end = 1;
		pthread_mutex_unlock(&init->death_mutex);
	}
	if (end == 1)
	{
		pthread_mutex_lock(&init->death_mutex);
		print_action(philo, init, philo->philo_id, "died");
		cleanup_all_mutex(init);
		pthread_mutex_unlock(&init->death_mutex);
	}
}

void	check_all_deaths(t_init *init)
{
	int	i;

	i = 0;
	while (i < init->nb_of_philo)
	{
		check_death(init, &init->philo[i]);
		i++;
	}
}
