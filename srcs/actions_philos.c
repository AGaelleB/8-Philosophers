/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:50:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/23 14:59:20 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	action_think(t_philo *philo)
{
	printf("%lld %d is thinking\n", timestamp(), philo->philo_id);
}

void	action_eat(t_philo *philo)
{
	t_init	*data;

	// ajouter aussi un check de si le temps entre 2 repas a ete respecté, sinon dead

	if (data->nb_must_eat != 0)
	{
		philo->nb_time_eat++;
		if (philo->nb_time_eat >= data->nb_must_eat)
		{
			printf("Philosopher %d a mangé %d fois. Fin de la simulation.\n", philo->philo_id, philo->nb_time_eat);
			// ajouter ici la logique pour terminer la simulation
			// définir un flag pour indiquer la fin de la simulation
			// ou utiliser un mécanisme de synchro pour informer les autres threads
			// que la simulation doit se terminer.
		}
	}

	printf("%lld %d is eating\n", timestamp(), philo->philo_id);
	philo->time_last_eat = timestamp();
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
	printf("%lld %d has taken a fork\n", timestamp(),philo->philo_id);
	action_eat(philo);
	action_drop_fork(philo);

}
void	action_sleep(t_philo *philo)
{

	printf("%lld %d is sleeping\n", timestamp(),philo->philo_id);
}

void	check_if_philo_died(t_philo *philo)
{
	/*
	Un philosophe meurt s'il n'a pas commencé à manger dans l'intervalle de 
	time_to_die. 
	Creer une autre boucle dans la fonction principale qui vérifie continuellement
	si chaque philosophe a commencé à manger dans le temps imparti
	*/

}