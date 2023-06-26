/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/26 18:20:12 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void *thread_routine(void *data)
{
	t_init	*init;
	t_philo	*philo;

	philo = (t_philo*)data;
	init = (t_init*)data;
	

	printf("Debut de la routine\n");
	
	action_think(philo);
	action_grab_fork(philo, init);
	action_eat(philo, init);
	action_drop_fork(philo, init);
	action_sleep(philo);
	return (NULL);
}


void	run_routine_philo(t_init *data)
{
	int	i;
	long long time_init;

	// j init mon time ici
	time_init = get_time();
	// printf("%time_init = %lld\n%s", BLUE, time_init, RESET);

	i = data->nb_of_philo - 1;
	// create threads and pass the specific philosopher to each thread
	while(i >= 0)
	{
		data->philo[i].time_init = time_init;
		// data->philo[i].init_data = data; // utile ? 
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
