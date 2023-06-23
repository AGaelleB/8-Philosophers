/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:19 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/23 13:01:37 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	action_think(t_philo *philo)
{

	printf("timestamp_in_ms %d is thinking", philo->philo_id);
}

void	action_eat(t_philo *philo)
{
	// faire une update de time_last_eat
	printf("timestamp_in_ms %d is eating", philo->philo_id);
}

void	action_drop_fork(t_philo *philo)
{
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_unlock(&philo->left_fork_id);
		pthread_mutex_unlock(&philo->right_fork_id);
	}
	else
	{
		pthread_mutex_unlock(&philo->right_fork_id);
		pthread_mutex_unlock(&philo->left_fork_id);
	}
}

void	action_grab_fork(t_philo *philo)
{
	// Philo prend d'abord la fourchette avec le plus petit identifiant
	if (philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&philo->left_fork_id);
		pthread_mutex_lock(&philo->right_fork_id);
	}
	else 
	{
		pthread_mutex_lock(&philo->right_fork_id);
		pthread_mutex_lock(&philo->left_fork_id);
	}
	printf("timestamp_in_ms %d has taken a fork", philo->philo_id);
	action_eat(philo);
	action_drop_fork(philo);

}
void	action_sleep(t_philo *philo)
{

	printf("timestamp_in_ms %d is sleeping", philo->philo_id);
}

// faire une fonction pour la mort des philos


void	check_if_philo_died(t_philo *philo)
{
	/*
	Un philosophe meurt s'il n'a pas commencé à manger dans l'intervalle de 
	time_to_die. 
	Creer une autre boucle dans la fonction principale qui vérifie continuellement
	si chaque philosophe a commencé à manger dans le temps imparti
	*/
}


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

int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;

	if(ac != 6 || ac != 5)
		return (write_error("Wrong numbers of arguments"));
	data = init_recup_data(data, ac, av);
	if (data == NULL)
		return (write_error("Failed to initialize data"));
	data = init_philo(data);
	if (data == NULL)
		return (write_error("Failed to initialize philo"));
	data = init_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize mutex"));

	run_routine_philo(data);
	return (0);
}
