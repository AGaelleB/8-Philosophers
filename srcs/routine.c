/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:11 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/28 15:57:31 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_if_philo_died(t_philo *philo, t_init *init) // OK
{
	/*
	Un philosophe meurt s'il n'a pas commencé à manger dans l'intervalle de 
	time_to_die. 
	*/

	// printf("%s time_last_eat = %lld\n%s", BLUE, philo->time_last_eat, RESET);
	// printf("%s time_to_die = %d\n%s", YELLOW, init->time_to_die, RESET);

	if (philo->time_last_eat >= init->time_to_die)
	{
		print_action(philo, philo->philo_id, "died");
		philo->flag_philo_died = 1;
		return (-1);
	}
	else
		return (0);

	// mettre fin au programme des le 1er philo died et ne pas afficher les autre died
}

void *thread_routine(void *arg)
{
	t_data *data = (t_data *)arg;

	while (check_if_philo_died(data->philo, data->init) == 0 && !data->philo->flag_philo_died)
	{
		// Vérifiez si la simulation est terminée avant chaque action.
		if (data->philo->flag_philo_died)
			break;
		action_grab_fork(data->philo, data->init);
		if (data->philo->flag_philo_died)
			break;
		action_eat(data->philo, data->init);
		if (data->philo->flag_philo_died)
			break;
		action_drop_fork(data->philo, data->init);
		if (data->philo->flag_philo_died)
			break;
		action_sleep(data->philo, data->init);
		if (data->philo->flag_philo_died)
			break;
		action_think(data->philo, data->init);

		if (check_if_philo_died(data->philo, data->init) == -1)
		{
			data->philo->flag_philo_died = 1;  // Indiquer la fin de la simulation.
			break;
		}
	}
	return (NULL);
}

void	run_routine_philo(t_init *init)
{
	int	i;
	long long time_init;
	t_data *data;

	// j init mon time ici
	time_init = get_time_philo();

	// creation des threads par philosophers
	i = init->nb_of_philo - 1;
	while(i >= 0)
	{
		data = malloc(sizeof(t_data));
		if (data == NULL)
			return ;
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_init = time_init;
		// data->philo[i].init_data = init; // utile ? 
		pthread_create(&init->philo[i].thread_philo, NULL, thread_routine, data);
		i--;
	}
	
	i = init->nb_of_philo - 1;
	// pthread_join attend que tous les threads se terminent avant de continuer
	while (i >= 0) 
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		i--;
	}
}




