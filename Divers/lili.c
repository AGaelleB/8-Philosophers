les threads en attente affiche quand meme leur message apres "printf("STOP PLEASE\n");" alors que je veux que tout s arrete, meme les autres threads en attente. Comment modifier mon code pour regler ce probleme ? 

voici les resultats d execution : 
 ./philo 200 1 1 1 
0 1 has taken a fork
1 2 has taken a fork
1 4 has taken a fork
1 5 has taken a fork
1 5 has taken a fork
1 5 is eating
1 3 has taken a fork
1 7 has taken a fork
1 8 has taken a fork
1 8 has taken a fork
1 8 is eating
1 10 has taken a fork
1 10 has taken a fork
1 10 is eating
1 12 has taken a fork
1 12 has taken a fork
1 12 is eating
1 14 has taken a fork
1 14 has taken a fork
1 14 is eating
1 16 has taken a fork
1 16 has taken a fork
1 16 is eating
1 18 has taken a fork
1 18 has taken a fork
1 18 is eating
1 20 has taken a fork
1 20 has taken a fork
1 20 is eating
1 23 has taken a fork
1 23 has taken a fork
1 23 is eating
1 22 has taken a fork
1 25 has taken a fork
1 25 has taken a fork
1 25 is eating
1 27 has taken a fork
1 27 has taken a fork
1 27 is eating
2 29 has taken a fork
2 29 has taken a fork
2 29 is eating
2 31 has taken a fork
2 31 has taken a fork
2 31 is eating
2 33 has taken a fork
2 33 has taken a fork
2 33 is eating
2 35 has taken a fork
2 35 has taken a fork
2 35 is eating
2 5 is sleeping
2 4 has taken a fork
2 4 is eating
2 6 has taken a fork
2 37 has taken a fork
2 37 has taken a fork
2 37 is eating
2 8 is sleeping
2 39 has taken a fork
2 7 has taken a fork
2 7 is eating
2 10 is sleeping
2 11 has taken a fork
2 39 has taken a fork
2 39 is eating
2 42 has taken a fork
2 12 is sleeping
2 9 has taken a fork
2 9 has taken a fork
2 9 is eating
2 14 is sleeping
2 15 has taken a fork
2 15 has taken a fork
2 15 is eating
2 42 has taken a forkmake register
2 42 is eating
2 47 has taken a fork
2 47 has taken a fork
2 13 has taken a fork
2 19 has taken a fork
2 16 is sleeping
2 17 has taken a fork
2 17 has taken a fork
2 17 is eating
2 44 has taken a fork
2 20 is sleeping
2 11 has taken a fork
2 11 is eating
2 13 has taken a fork
2 13 is eating
2 45 has taken a fork
2 41 has taken a fork
2 21 has taken a fork
2 49 has taken a fork
2 19 has taken a fork
3 19 died
STOP PLEASE
3 18 is sleeping
3 50 has taken a fork
3 47 is eating
3 51 has taken a fork
3 23 is sleeping
3 22 has taken a fork
3 46 has taken a fork
3 24 has taken a fork
3 52 has taken a fork
3 53 has taken a fork
3 25 is sleeping
3 54 has taken a fork

voici mon code : 
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
			printf("STOP PLEASE\n");
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

void	action_think(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init); // NEW
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;

	print_action(init, philo->philo_id, "is thinking");
	usleep(init->time_to_think * 1000);

	check_time_for_philo_to_die(philo, init); // NEW
	if (check_flag_died(init) || check_flag_all_eat(init)) // NEW
		return ; // NEW
}

void	action_sleep(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init); // NEW
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;

	print_action(init, philo->philo_id, "is sleeping");
	usleep(init->time_to_sleep * 1000);

	check_time_for_philo_to_die(philo, init); // NEW
	if (check_flag_died(init) || check_flag_all_eat(init)) // NEW
		return ; // NEW
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	if(philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else if(philo->left_fork_id > philo->right_fork_id)
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
}

void	action_eat(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init); // NEW
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;

	print_action(init, philo->philo_id, "is eating");
	pthread_mutex_lock(&(philo->eat_mutex));
	init->all_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	if (init->nb_must_eat != 0
		&& ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		pthread_mutex_lock(&(init->flag_all_eat_mutex));
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	philo->time_last_eat = get_time_philo();
	usleep(init->time_to_eat * 1000);

	check_time_for_philo_to_die(philo, init); // NEW
	if (check_flag_died(init) || check_flag_all_eat(init)) // NEW
		return ; // NEW
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init); // NEW
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if(philo->left_fork_id < philo->right_fork_id)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		action_eat(philo, init);
		action_drop_fork(philo, init);
	}
	else if(philo->left_fork_id > philo->right_fork_id)
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		check_time_for_philo_to_die(philo, init);
		print_action(init, philo->philo_id, "has taken a fork");
		action_eat(philo, init);
		action_drop_fork(philo, init);
	}
	check_time_for_philo_to_die(philo, init); // NEW
	if (check_flag_died(init) || check_flag_all_eat(init)) // NEW
		return ; // NEW
}

void	*thread_run(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (data->init->flag_died != 1 || data->init->flag_all_eat != 1)
	{
		if (check_flag_died(data->init) || check_flag_all_eat(data->init))
			break ;
		action_take_fork(data->philo, data->init);
		if (check_flag_died(data->init) || check_flag_all_eat(data->init))
			break ;
		action_sleep(data->philo, data->init);
		if (check_flag_died(data->init) || check_flag_all_eat(data->init))
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
