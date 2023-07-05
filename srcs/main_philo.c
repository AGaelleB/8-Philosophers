/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:19 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/05 16:50:33 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_init	*initialize_all(t_init *data, int ac, char **av)
{
	data = init_recup_data(data, ac, av);
	if (!data)
		return (NULL);
	data = init_philo(data);
	if (!data)
		return (NULL);
	data = init_mutex(data);
	if (!data)
		return (NULL);
	data = init_write_mutex(data);
	if (!data)
		return (NULL);
	data = init_eat_count_mutex(data);
	if (!data)
		return (NULL);
	data = init_death_mutex(data);
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
	if (ac != 6 && ac != 5)
		return (write_error("Wrong numbers of arguments"));
	if (!ft_isdigit_str(av[1]) || !ft_isdigit_str(av[2])
		|| !ft_isdigit_str(av[3]) || !ft_isdigit_str(av[4])
		|| (ac == 6 && !ft_isdigit_str(av[5])))
		return (write_error("Invalid value"));
	data = initialize_all(data, ac, av);
	if (!data)
		return (write_error("Failed to initialize"));
	run_routine_philo(data);
	wait_threads(data);
	cleanup_forks(data);
	cleanup_all_mutex(data);
	return (0);
}
