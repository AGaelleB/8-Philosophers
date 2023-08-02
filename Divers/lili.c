Normalement la commande ./philo 200 2147483647 89 10 ne devrait pas s executer mais elle s execute, pourquoi ? 

Voici mon code : 

# define INT_MAX 2147483647

void	print_action(t_init *init, int id, char *str)
{
	if (check_flag_all_eat(init))
		return ;
	if (check_flag_died(init))
	{
		pthread_mutex_lock(&init->death_printed_mutex);
		if (init->death_printed == 0)
		{
			init->death_printed = 1;
			pthread_mutex_unlock(&init->death_printed_mutex);
			pthread_mutex_lock(&init->write_mutex);
			printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
			printf("died\n");
			pthread_mutex_unlock(&init->write_mutex);
		}
		else
			pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
	pthread_mutex_lock(&init->write_mutex);
	printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
	printf("%s\n", str);
	pthread_mutex_unlock(&init->write_mutex);
}

t_init	*init_recup_data(t_init *init, int ac, char **av)
{
	(void)ac;
	init = malloc(sizeof(t_init));
	if (init == NULL)
		return (NULL);
	init->nb_of_philo = ft_atoi_philo(av[1]);
	init->time_to_die = ft_atoi_philo(av[2]);
	init->time_to_eat = ft_atoi_philo(av[3]);
	init->time_to_sleep = ft_atoi_philo(av[4]);
	if (init->time_to_die > INT_MAX || init->time_to_eat > INT_MAX
		|| init->time_to_sleep > INT_MAX)
	{
		free(init);
		return (NULL);
	}
	init->all_eat = 0;
	init->flag_all_eat = 0;
	init->flag_died = 0;
	init->death_printed = 0;
	calculate_and_set_time_to_think(init);
	check_and_set_nb_must_eat(init, ac, av);
	return (init);
}
