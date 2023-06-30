/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:19 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/30 10:13:55 by abonnefo         ###   ########.fr       */
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
	data = init_philo(data);
	if (data == NULL)
		return (write_error("Failed to initialize philo"));
	data = init_mutex(data);
	if (data == NULL)
		return (write_error("Failed to initialize mutex"));
	data = init_forks(data);
	if (data == NULL)
		return (write_error("Failed to initialize forks"));
	run_routine_philo(data);

	cleanup_forks(data);
	return (0);
}
