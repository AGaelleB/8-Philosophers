/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 10:30:19 by abonnefo          #+#    #+#             */
/*   Updated: 2023/06/21 17:15:03 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
Chaque programme doit prendre les arguments suivants :

- number_of_philosophers 
- time_to_die 
- time_to_eat 
- time_to_sleep
- [number_of_times_each_philosopher_must_eat]
*/

#include "../includes/philosophers.h"

int	main(int ac, char **av)
{
	if(ac != 6)
		return (write_error("Wrong amount of arguments"));
	
	return (0);
}
