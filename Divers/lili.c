je devrais touhours avoir un died vers 310 avec cette commande mais parfois je died a 400, pourquoi ? 

./philo 4 310 200 100
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
0 3 has taken a fork
0 3 has taken a fork
0 3 is eating
200 1 is sleeping
200 2 has taken a fork
200 2 has taken a fork
200 2 is eating
200 3 is sleeping
200 4 has taken a fork
200 4 has taken a fork
200 4 is eating
300 3 is thinking
300 1 is thinking
311 1 died
➜  8-Philosophers git:(main) ✗ ./philo 4 310 200 100
1 1 has taken a fork
1 1 has taken a fork
1 1 is eating
1 4 has taken a fork
201 1 is sleeping
201 4 has taken a fork
201 4 is eating
201 2 has taken a fork
201 2 has taken a fork
201 2 is eating
301 1 is thinking
312 1 died
➜  8-Philosophers git:(main) ✗ ./philo 4 310 200 100
0 2 has taken a fork
1 2 has taken a fork
1 2 is eating
1 3 has taken a fork
201 2 is sleeping
201 1 has taken a fork
201 1 has taken a fork
201 1 is eating
201 3 has taken a fork
201 3 is eating
301 2 is thinking
401 1 is sleeping
401 2 died

mon code : 


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
	long long	time_last_eat;

	pthread_mutex_lock(&philo->time_last_eat_mutex);
	time_last_eat = philo->time_last_eat;
	pthread_mutex_unlock(&philo->time_last_eat_mutex);
	if ((get_time_philo() - time_last_eat) > init->time_to_die)
	{
		pthread_mutex_lock(&(init->flag_died_mutex));
		init->flag_died = 1;
		pthread_mutex_unlock(&(init->flag_died_mutex));
		print_action(init, philo->philo_id, "died");
		return (1);
	}
	return (0);
}


void	ft_usleep(long long duration, t_init *init)
{
	long long	start;

	start = get_time_philo();
	while (!check_time_for_philo_to_die(init->philo, init))
	{
		if (get_time_philo() - start >= duration)
			break ;
		usleep(init->nb_of_philo * 2);
	}
}

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

void	action_think(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if (init->nb_of_philo > 1)
	{
		print_action(init, philo->philo_id, "is thinking");
		ft_usleep(init->time_to_think, init);
	}
}

void	action_sleep(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if (init->nb_of_philo > 1)
	{
		print_action(init, philo->philo_id, "is sleeping");
		ft_usleep(init->time_to_sleep, init);
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
	if (init->nb_must_eat != 0
		&& ((init->nb_of_philo * init->nb_must_eat) == init->all_eat))
	{
		init->flag_all_eat = 1;
		pthread_mutex_unlock(&(init->flag_all_eat_mutex));
		return ;
	}
	pthread_mutex_unlock(&(init->flag_all_eat_mutex));
	philo->time_last_eat = get_time_philo();
	ft_usleep(init->time_to_eat, init);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	check_time_for_philo_to_die(philo, init);
	if (check_flag_died(init) || check_flag_all_eat(init))
		return ;
	if (init->nb_of_philo > 1)
	{
		if (philo->philo_id % 2 == 0)
		{
			pthread_mutex_lock(&init->forks[philo->left_fork_id]);
			check_time_for_philo_to_die(philo, init); // au lieu d avoir died a 400 pour ./philo 4 310 200 100 j'ai 500
			print_action(init, philo->philo_id, "has taken a fork");
			pthread_mutex_lock(&init->forks[philo->right_fork_id]);
			// check_time_for_philo_to_die(philo, init);
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
			// check_time_for_philo_to_die(philo, init);
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

