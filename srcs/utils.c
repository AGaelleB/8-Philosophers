/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 14:40:03 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/29 11:35:23 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
int	write_error(char *str)
{
	int	len;

	len = 0;
	while(str[len])
		len++;
	write(2, "Error: ", 7);
	write(2, str, len);
	write(2, "\n", 1);
	return (0);
}

long long	get_time_philo(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);

	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000)); // - time start ? 
	// 1 sec = 1 000 milisecondes
}

void	print_action(t_philo *philo, int id, char *str)
{
	// Vérifiez si la simulation est terminée.
	// if (init->end_flag != 1) 
	// {
		printf("%lli ", get_time_philo() - philo->time_init);
		printf("%i ", id); // + 1);
		printf("%s\n", str);
		
	// }
}

void cleanup_forks(t_init *data)
{
	int	i;

	if (data->forks != NULL)
	{
		i = data->nb_of_philo - 1;
		while (i >= 0)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i--;
		}

		free(data->forks);
		data->forks = NULL;
	}
}


// void	ft_free_tab(char **tab)
// {
// 	int	i;

// 	i = 0;
// 	if (tab == NULL)
// 		return (NULL);
// 	while (tab[i])
// 	{
// 		free(tab[i]);
// 		i++;
// 	}
// 	free(tab);
// }
