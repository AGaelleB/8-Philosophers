/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:19 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/26 12:29:29 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_init *initialize_data_and_mutex(t_init *data, int ac, char **av)
{
	data = init_recup_data(data, ac, av);
	if (!data)
		return (NULL);
	data = init_philo(data);
	if (!data)
		return (NULL);
	data = init_write_mutex(data);
	if (!data)
		return (NULL);
	data = init_death_mutex(data);
	if (!data)
		return (NULL);
	data = init_forks(data);
	if (!data)
		return (NULL);
	return data;
}

int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;
	if (ac != 6 && ac != 5)
		return (write_error("Wrong numbers of arguments"));
	if (!ft_isdigit_str(av[1]) || !ft_isdigit_str(av[2])
		|| !ft_isdigit_str(av[3]) || !ft_isdigit_str(av[4])
		|| (ac == 6 && !ft_isdigit_str(av[5])))
		return (write_error("Invalid value"));
	data = initialize_data_and_mutex(data, ac, av);
	if (!data)
	{
		free(data);
		free_all_mutex_and_forks(data);
		return (write_error("Failed to initialize"));
	}
	run_routine_philo(data);
	int i = 0;
	while (i < data->nb_of_philo)
	{
		free_data(data->philo[i].data);
		i++;
	}
	free_all_mutex_and_forks(data);
	free(data);
	return (0);
}

