comment faire pour m'assurer que les threads en attentes n'imprime pas une fois qu un philo died 
j utilise deja plusieurs flags mais ca ne fonctionne pas bien. faut il ajouter des verifications quelque part ? 

 ./philo 200 1 1 1 
2 action_take_fork = 0
0 1 has taken a fork
2 action_take_fork = 0
0 1 has taken a fork
 action_eat = 0
0 1 is eating
2 action_take_fork = 0
0 3 has taken a fork
2 action_take_fork = 0
0 3 has taken a fork
 action_eat = 0
0 3 is eating
2 action_take_fork = 0
0 5 has taken a fork
2 action_take_fork = 0
0 5 has taken a fork
 action_eat = 0
0 5 is eating
[...]
1 action_take_fork = 0
1 6 has taken a fork
1 action_take_fork = 0
2 action_take_fork = 0
1 7 is sleeping
 action_eat = 0
1 6 is eating
1 8 has taken a fork
1 action_take_fork = 0
1 action_take_fork = 0
 action_sleep = 0
2 action_take_fork = 0
1 8 has taken a fork
 action_eat = 0
 action_sleep = 0
1 9 is sleeping
1 action_take_fork = 0
1 13 is sleeping
 action_sleep = 0
1 23 is sleeping
2 action_take_fork = 0
1 action_take_fork = 0
1 98 has taken a fork
2 25 died
2 14 has taken a fork
2 10 has taken a fork
2 12 has taken a fork
2 101 has taken a fork
2 11 is sleeping
2 3 is sleeping
2 103 has taken a fork
2 95 has taken a fork
2 16 has taken a fork
2 15 is sleeping
2 4 is eating
2 17 is sleeping
2 105 has taken a fork
2 18 has taken a fork
2 107 has taken a fork
2 20 has taken a fork
2 19 is sleeping
2 99 has taken a fork
2 109 has taken a fork
2 8 is eating
2 21 is sleeping
2 22 has taken a fork
2 111 has taken a fork
2 24 has taken a fork


voici mon code :

int	write_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
	
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
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_action(t_init *init, int id, char *str)
{
	if (check_flag_all_eat(init))
		return ;
	if (check_flag_died(init))
	{
		pthread_mutex_lock(&init->death_printed_mutex);
		if (init->death_printed == 0)
		{
			init->death_printed++;
			pthread_mutex_unlock(&init->death_printed_mutex);
			pthread_mutex_lock(&init->write_mutex);
			printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
			printf("%sdied%s\n", RED, RESET);
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
	if (ac == 6 && strcmp(av[5], "0") == 0)
	{
		write_error("Number of meals must be > 0");
		return (1);
	}
	return (0);
}

int	check_flag_died(t_init *init)
{
	pthread_mutex_lock(&(init->flag_died_mutex));
	if (init->flag_died == 1)
	{
		pthread_mutex_unlock(&init->flag_died_mutex);
		return (1);
	}
	pthread_mutex_unlock(&init->flag_died_mutex);
	return (0);
}

int	check_flag_all_eat(t_init *init)
{
	pthread_mutex_lock(&init->flag_all_eat_mutex);
	if (init->flag_all_eat == 1)
	{
		pthread_mutex_unlock(&init->flag_all_eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(&init->flag_all_eat_mutex);
	return (0);
}

int	check_flag_death_printed(t_init *init)
{
	pthread_mutex_lock(&init->death_printed_mutex);
	if (init->death_printed == 1)
	{
		pthread_mutex_unlock(&init->death_printed_mutex);
		return (1);
	}
	pthread_mutex_unlock(&init->death_printed_mutex);
	return (0);
}

int	check_time_for_philo_to_die(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		pthread_mutex_lock(&(init->flag_died_mutex));
		init->flag_died = 1;
		pthread_mutex_unlock(&(init->flag_died_mutex));
		print_action(init, philo->philo_id, "died");
		return (1);
	}
	return (0);
}

void action_think(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
	if (init->death_printed == 0)
	{
		printf("%s action_think = %d%s\n",YELLOW, init->death_printed, RESET );
		print_action(init, philo->philo_id, "is thinking");
	}
	usleep(init->time_to_think * 1000);

}

void action_sleep(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
	if (init->death_printed == 0)
	{
		printf("%s action_sleep = %d%s\n",YELLOW, init->death_printed, RESET );
		print_action(init, philo->philo_id, "is sleeping");
	}
	usleep(init->time_to_sleep * 1000);
}

void action_drop_fork(t_philo *philo, t_init *init)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else if (philo->philo_id % 2 != 0)
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
}

void action_eat(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
	if (init->death_printed == 0)
	{
		printf("%s action_eat = %d%s\n",YELLOW, init->death_printed, RESET );
		print_action(init, philo->philo_id, "is eating");
	}
	pthread_mutex_lock(&(philo->eat_mutex));
	init->all_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	if (init->nb_must_eat != 0 && ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		pthread_mutex_lock(&(init->flag_all_eat_mutex));
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return;
	}
	philo->time_last_eat = get_time_philo();
	usleep(init->time_to_eat * 1000);
}

void action_take_fork(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init) || check_flag_death_printed(init))
		return;
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		if (init->death_printed == 0)
		{
			printf("%s1 action_take_fork = %d%s\n",YELLOW, init->death_printed, RESET );
			print_action(init, philo->philo_id, "has taken a fork");
		}
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		if (init->death_printed == 0)
		{
			printf("%s1 action_take_fork = %d%s\n",YELLOW, init->death_printed, RESET );
			print_action(init, philo->philo_id, "has taken a fork");
		}
		action_eat(philo, init);
		action_drop_fork(philo, init);
	}
	else if (philo->philo_id % 2 != 0)
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		if (init->death_printed == 0)
		{
			printf("%s2 action_take_fork = %d%s\n",YELLOW, init->death_printed, RESET );
			print_action(init, philo->philo_id, "has taken a fork");
		}
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		if (init->death_printed == 0)
		{
			printf("%s2 action_take_fork = %d%s\n",YELLOW, init->death_printed, RESET );
			print_action(init, philo->philo_id, "has taken a fork");
		}
		action_eat(philo, init);
		action_drop_fork(philo, init);
	}
}

void	*thread_run(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1) //(data->init->flag_died != 1 || data->init->flag_all_eat != 1) // || data->init->death_printed != 1)
	{
		if (check_flag_died(data->init) || check_flag_all_eat(data->init) || check_flag_death_printed(data->init))
			break ;
		action_take_fork(data->philo, data->init);
		if (check_flag_died(data->init) || check_flag_all_eat(data->init) || check_flag_death_printed(data->init))
			break ;
		action_sleep(data->philo, data->init);
		if (check_flag_died(data->init) || check_flag_all_eat(data->init) || check_flag_death_printed(data->init))
			break ;
		action_think(data->philo, data->init);
	}
	return (NULL);
}

void	init_and_create_threads(t_init *init, long long int time_init)
{
	t_data	*data;
	int		i;

	i = 0;
	while ((i < init->nb_of_philo))
	{
		data = malloc(sizeof(t_data));
		if (data == NULL)
			return ;
		data->init = init;
		data->philo = &init->philo[i];
		data->philo->time_last_eat = get_time_philo();
		init->philo[i].data = data;
		data->philo->time_init = time_init;
		if (pthread_create(&init->philo[i].thread_philo, NULL,
				thread_run, data))
			return ;
		i++;
	}
}

void	run_routine_philo(t_init *init)
{
	long long int	time_init;
	int				i;

	time_init = get_time_philo();
	i = 0;
	init_and_create_threads(init, time_init);
	while ((i < init->nb_of_philo))
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		free(init->philo[i].data);
		i++;
	}
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

