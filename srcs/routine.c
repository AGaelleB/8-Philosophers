/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/23 14:52:22 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*thread_routine(void *data)
{
	t_philo	*philo;
	
	philo = (t_philo*)data;
	
	printf("Debut de la routine\n");
	action_think(data);
	action_grab_fork(data);
	action_eat(data);
	action_drop_fork(data);
	action_sleep(data);
	return (NULL);
}

void	run_routine_philo(t_init *data)
{
	int	i;

	i = data->nb_of_philo - 1;
	// create threads and pass the specific philosopher to each thread
	while(i >= 0)
	{
		pthread_create(&data->philo[i].thread_philo, NULL, thread_routine, &data->philo[i]);
		i--;
	}
	
	i = data->nb_of_philo - 1;
	// pthread_join attend que tous les threads se terminent avant de continuer
	while (i >= 0) 
	{
		pthread_join(data->philo[i].thread_philo, NULL);
		i--;
	}
}