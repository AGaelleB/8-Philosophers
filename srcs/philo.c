/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:19 by abonnefo          #+#    #+#             */
/*   Updated: 2023/07/05 12:07:41 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"


int	main(int ac, char **av)
{
	t_init	*data;

	data = NULL;
	if(ac != 6 && ac != 5)
		return (write_error("Wrong numbers of arguments"));
	data = init_recup_data(data, ac, av);
	if (data == NULL)
		return (write_error("Failed to initialize data"));
	if (ac == 6 && (data->nb_must_eat == 0))
	{
		free(data);
		return write(2, "", 1);
	}
	if (ac == 6 && (data->nb_must_eat < 0 || !ft_isdigit_str(av[5])))
	{
		free(data);
		return (write_error("Failed to use negative value"));
	}
	data = init_philo(data);
	if (data == NULL)
		return (write_error("Failed to initialize philo"));
	data = init_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize mutex"));
	data = init_write_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize write_mutex"));
	data = init_eat_count_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize eat_count_mutex"));
	data = init_forks(data);
	if (data == NULL)
		return (write_error("Failed to initialize forks"));
	run_routine_philo(data);
	wait_threads(data);
	cleanup_forks(data);
	cleanup_all_mutex(data);
	return (0);
}
