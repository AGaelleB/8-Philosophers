/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:19 by abonnefo          #+#    #+#             */
/*   Updated: 2023/08/04 13:16:53 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	write_error_and_return(char *str)
{
	write_error(str);
	return (1);
}

int	check_before_initialize(int ac, char **av)
{
	if (ac != 6 && ac != 5)
		return (write_error_and_return("Wrong numbers of arguments"));
	if (!ft_isdigit_str(av[1]) || !ft_isdigit_str(av[2])
		|| !ft_isdigit_str(av[3]) || !ft_isdigit_str(av[4])
		|| (ac == 6 && !ft_isdigit_str(av[5])))
		return (write_error_and_return("Invalid value"));
	if (ft_atoi_philo(av[1]) == 0)
		return (write_error_and_return("Number of Philo must be > 0"));
	if (ft_atoi_philo(av[1]) > INT_MAX || ft_atoi_philo(av[2]) > INT_MAX
		|| ft_atoi_philo(av[3]) > INT_MAX || ft_atoi_philo(av[4]) > INT_MAX
		|| (ac == 6 && ft_atoi_philo(av[5]) > INT_MAX))
		return (write_error_and_return("Invalid value"));
	if (ac == 6 && ft_atoi_philo(av[5]) == 0)
		return (write_error_and_return("Number of meals must be > 0"));
	return (0);
}

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
	data = init_death_printed_mutex(data);
	if (!data)
		return (NULL);
	data = init_forks(data);
	if (!data)
		return (NULL);
	return (data);
}

int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;
	if (check_before_initialize(ac, av) == 0)
	{
		data = initialize_data_and_mutex(data, ac, av);
		if (data == NULL)
			return (write_error("Failed to initialize"));
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
