le temps pour ma mort n est pas bon, j'ai : 

./philo 3 400 100 5600
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
100 1 is sleeping
100 3 has taken a fork
101 3 has taken a fork
101 3 is eating
101 2 has taken a fork
201 3 is sleeping
201 2 has taken a fork
201 2 is eating
301 2 is sleeping
5700 1 died

alors que je devrais avoir : 

./philo 3 400 100 5600
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
100 1 is sleeping
100 3 has taken a fork
101 3 has taken a fork
101 3 is eating
101 2 has taken a fork
201 3 is sleeping
201 2 has taken a fork
201 2 is eating
301 2 is sleeping
400 1 died

Peux tu me montrer ou et comment changer mon code ? 


long long	get_time_philo(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_if_philosopher_death(t_init *init, int id)
{
	pthread_mutex_lock(&init->death_printed_mutex);
	if (check_flag_died(init))
	{
		if (init->death_printed == 0)
		{
			init->death_printed++;
			pthread_mutex_lock(&init->write_mutex);
			printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
			printf("%lld %d ", (init->philo->time_init - init->philo->time_last_eat + init->time_to_die), id);
			printf("died\n");
			pthread_mutex_unlock(&init->write_mutex);
		}
		pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
	pthread_mutex_unlock(&init->death_printed_mutex);
}

void	print_action(t_init *init, int id, char *str)
{
	print_if_philosopher_death(init, id);
	pthread_mutex_lock(&init->death_printed_mutex);
	if (check_flag_all_eat(init))
	{
		pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
	pthread_mutex_unlock(&init->death_printed_mutex);
	pthread_mutex_lock(&init->death_printed_mutex);
	if (init->death_printed == 0)
	{
		pthread_mutex_lock(&init->write_mutex);
		printf("%lld %d ", (get_time_philo() - init->philo->time_init), id);
		printf("%s\n", str);
		pthread_mutex_unlock(&init->write_mutex);
		pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
	else
	{
		pthread_mutex_unlock(&init->death_printed_mutex);
		return ;
	}
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


void	action_think(t_philo *philo, t_init *init)
{
	if (init->nb_of_philo > 1) // Check ici
	{
		check_time_for_philo_to_die(philo, init);
		if (check_flag_died(init) || check_flag_all_eat(init))
			return ;
		print_action(init, philo->philo_id, "is thinking");
		usleep(init->time_to_think * 1000);
	}
}

void	action_sleep(t_philo *philo, t_init *init)
{
	if (init->nb_of_philo > 1) // Check ici
	{
		check_time_for_philo_to_die(philo, init);
		if (check_flag_died(init) || check_flag_all_eat(init))
			return ;
		print_action(init, philo->philo_id, "is sleeping");
		usleep(init->time_to_sleep * 1000);
	}
}

void	action_drop_fork(t_philo *philo, t_init *init)
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

void	action_eat(t_philo *philo, t_init *init)
{
	print_action(init, philo->philo_id, "is eating");
	pthread_mutex_lock(&(init->flag_all_eat_mutex));
	init->all_eat++;
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));
	pthread_mutex_lock(&(init->flag_all_eat_mutex));
	if (init->nb_must_eat != 0
		&& ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));
	philo->time_last_eat = get_time_philo();
	usleep(init->time_to_eat * 1000);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if (init->nb_of_philo > 1) // Check ici
	{
		if (philo->philo_id % 2 == 0)
		{
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			check_time_for_philo_to_die(philo, init);
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
		else if (philo->philo_id % 2 != 0)
		{
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			check_time_for_philo_to_die(philo, init);
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			print_action(init, philo->philo_id, "has taken a fork");
			action_eat(philo, init);
			action_drop_fork(philo, init);
		}
	}
}


void	*thread_run(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_flag_died(data->init) || check_flag_all_eat(data->init)
			|| check_flag_death_printed(data->init))
			return (NULL);
		action_take_fork(data->philo, data->init);
		if (check_flag_died(data->init) || check_flag_all_eat(data->init)
			|| check_flag_death_printed(data->init))
			return (NULL);
		action_sleep(data->philo, data->init);
		if (check_flag_died(data->init) || check_flag_all_eat(data->init)
			|| check_flag_death_printed(data->init))
			return (NULL);
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
