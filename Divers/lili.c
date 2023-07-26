
void	action_think(t_philo *philo, t_init *init)
{
	if (check_and_stop_if_philo_died(philo, init))
		return;
	print_action(init, philo->philo_id, "is thinking");
	usleep(init->time_to_think * 1000);
}

void	action_sleep(t_philo *philo, t_init *init)
{
	if (check_and_stop_if_philo_died(philo, init))
		return;
	print_action(init, philo->philo_id, "is sleeping");
	usleep(init->time_to_sleep * 1000);
}

void	action_drop_fork(t_philo *philo, t_init *init)
{
	if (check_and_stop_if_philo_died(philo, init))
		return;

	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
	}
	else
	{
		pthread_mutex_unlock(&init->forks[philo->right_fork_id]);
		pthread_mutex_unlock(&init->forks[philo->left_fork_id]);
	}
}

void	action_eat(t_philo *philo, t_init *init)
{
	int	i;

	i = 0;
	print_action(init, philo->philo_id, "is eating");
	philo->time_last_eat = get_time_philo();
	pthread_mutex_lock(&(philo->eat_mutex));
	init->philo[i].nb_time_eat++;
	pthread_mutex_unlock(&(philo->eat_mutex));
	if (init->nb_must_eat != 0)
	{
		if ((init->nb_of_philo * init->nb_must_eat)
			== init->philo[i].nb_time_eat)
		{
			pthread_mutex_lock(&(init->end_flag_mutex));
			init->end_flag = 1;
			pthread_mutex_unlock(&(init->end_flag_mutex));
			return ;
		}
		init->all_eat++;
	}
	i++;
	usleep(init->time_to_eat * 1000);
}

void	action_take_fork(t_philo *philo, t_init *init)
{
	if (init->end_flag == 1)
		return;
	
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
	}
	else
	{
		pthread_mutex_lock(&init->forks[philo->right_fork_id]);
		pthread_mutex_lock(&init->forks[philo->left_fork_id]);
	}
	print_action(init, philo->philo_id, "has taken a fork");
	print_action(init, philo->philo_id, "has taken a fork");
}

void *thread_run(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (data->init->end_flag != 1)
	{
		action_take_fork(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init))
			break;
		action_eat(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init))
			break;
		action_drop_fork(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init))
			break;
		action_sleep(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init))
			break;
		action_think(data->philo, data->init);
		if (check_and_stop_if_philo_died(data->philo, data->init))
			break;
	}
	return (NULL);
}

int check_and_stop_if_philo_died(t_philo *philo, t_init *init)
{
	if ((get_time_philo() - philo->time_last_eat) > init->time_to_die)
	{
		print_action(init, philo->philo_id, "died");
		pthread_mutex_lock(&(init->end_flag_mutex));
		init->end_flag = 1;
		pthread_mutex_unlock(&(init->end_flag_mutex));
		return (1);
	}
	return (0);
}

void init_run_philosophers(t_init *init, long long int time_init)
{
	t_data *data;
	int i;

	i = 0;
	if (init->end_flag != 1)
	{
		while ((i < init->nb_of_philo))
		{
			data = malloc(sizeof(t_data));
			if (data == NULL)
				return;
			data->init = init;
			data->philo = &init->philo[i];
			data->philo->time_last_eat = get_time_philo();
			init->philo[i].data = data;
			data->philo->time_init = time_init;
			if (pthread_create(&init->philo[i].thread_philo, NULL, thread_run, data))
				return ;
			i++;
		}
	}
}

void run_routine_philo(t_init *init)
{
	long long int	time_init;
	int				i;

	time_init = get_time_philo();
	i = 0;
	init_run_philosophers(init, time_init);
	while (i < init->nb_of_philo)
	{
		pthread_join(init->philo[i].thread_philo, NULL);
		free(init->philo[i].data); // m'a supprime mes derniers leaks !!!
		i++;
	}
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
	data = initialize_data_and_mutex(data, ac, av);
	if (data == NULL)
	{
		free(data);
		return (write_error("Failed to initialize"));
	}
	run_routine_philo(data);
	if (data->end_flag)
	{
		free_all_mutex_and_forks(data);
		return (1);
	}
	free_all_mutex_and_forks(data);
	return (0);
}

