/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:19 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/31 14:44:44 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_init	*initialize_data_and_mutex(t_init *data, int ac, char **av)
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
	data = init_flag_died_mutex(data);
	if (!data)
		return (NULL);
	data = init_flag_all_eat_mutex(data);
	if (!data)
		return (NULL);
	data = init_death_printed(data);
	if (!data)
		return (NULL);
	data = init_forks(data);
	if (!data)
		return (NULL);
	return (data);
}

int	check_before_initialize(int ac, char **av)
{
	if (ac != 6 && ac != 5)
	{
		write_error("Wrong numbers of arguments");
		return (1);
	}
	if (!ft_isdigit_str(av[1]) || !ft_isdigit_str(av[2])
		|| !ft_isdigit_str(av[3]) || !ft_isdigit_str(av[4])
		|| (ac == 6 && !ft_isdigit_str(av[5])))
	{
		write_error("Invalid value");
		return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;
	if (check_before_initialize(ac, av) == 0)
	{
		data = initialize_data_and_mutex(data, ac, av);
		if (data == NULL)
		{
			free(data);
			return (write_error("Failed to initialize"));
		}
		if (check_single_philo_and_exit(data, av))
		{
			free_all_mutex_and_forks(data);
			return (1);
		}
		run_routine_philo(data);
		if (data->flag_died || data->flag_all_eat)
		{
			free_all_mutex_and_forks(data);
			return (1);
		}
		free_all_mutex_and_forks(data);
	}
	return (0);
}
